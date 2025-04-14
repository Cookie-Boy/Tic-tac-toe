#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <atomic>

#include <interface.hpp>

class MultiplayerManager {
public:

    bool isServerRole() const { return isServer; }
    int getSocket() const { return sockfd; }
    Figure getMyFigure() const { return myFigure; }
    Figure getOpponentFigure() const { return opponentFigure; }

    bool isReady() const { return ready; }

    void setIpList(std::vector<std::string> ipList) {
        this->ipList = ipList;
    }

    void setMyFigure(Figure myFigure) {
        this->myFigure = myFigure;
    }

    void setPort(int port) {
        this->port = port;
    }

    bool getReadyToContinue() {
        return this->readyToContinue;
    }

    void setReadyToContinue(bool readyToContinue) {
        this->readyToContinue = readyToContinue;
    }

    bool getEnemyReady() {
        return this->enemyReady;
    }

    void setEnemyReady(bool enemyReady) {
        this->enemyReady = enemyReady;
    }

    void initialize() {
        for (const auto& ip : ipList) {
            if (tryConnect(ip.c_str(), port, sockfd, myFigure, opponentFigure)) {
                isServer = false;
                ready = true;
                std::cout << "✅ Подключились к " << ip << " как клиент.\n";
                return;
            }
        }

        // Стали сервером
        isServer = true;
        serverThread = std::thread([this]() {
            sockfd = waitForClient(port, myFigure);
            opponentFigure = myFigure == Figure::Cross ? Figure::Zero : Figure::Cross;
            ready = true;
            std::cout << "🟢 Клиент подключился, начинаем игру.\n";
        });
        serverThread.detach();
    }

    void disconnect() {
        if (sockfd != -1) {
            shutdown(sockfd, SHUT_RDWR);
            close(sockfd);
            sockfd = -1;
            ready = false;
            enemyReady = false;
        }
    }

private:
    std::vector<std::string> ipList;
    int port;

    int sockfd = -1;
    bool isServer = false;
    Figure myFigure;
    Figure opponentFigure;

    std::atomic<bool> ready = false;
    std::thread serverThread;

    bool readyToContinue = false;
    bool enemyReady;

    bool tryConnect(const char* ip, int port, int& outSock, Figure myFigure, Figure& serverFigure) {
        int s = socket(AF_INET, SOCK_STREAM, 0);
        if (s < 0) return false;

        linger ling = {1, 0};
        setsockopt(s, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, ip, &addr.sin_addr);



        if (connect(s, (sockaddr*)&addr, sizeof(addr)) < 0) {
            std::cout << "🟡 Не удалось подключиться к серверу." << std::endl;
            close(s);
            return false;
        }

        // Отправляем желаемую фигуру
        uint8_t desired = static_cast<uint8_t>(myFigure);
        send(s, &desired, sizeof(desired), 0);

        // Получаем ответ (фигура сервера или отказ)
        uint8_t response;
        if (recv(s, &response, sizeof(response), 0) <= 0 || response == 0xFF) {
            std::cout << "⚠️ Фигуры совпали, такой сервер не подходит" << std::endl;
            close(s);
            sleep(1);
            return false;
        }

        serverFigure = static_cast<Figure>(response);
        outSock = s;
        return true;
    }

    int waitForClient(int port, Figure serverFigure) {
        while (true) {
            std::cout << "🟡 Ожидание подключения клиента...\n";
    
            int listener = socket(AF_INET, SOCK_STREAM, 0);
            if (listener < 0) {
                perror("socket");
                sleep(1);
                continue;
            }
    
            int opt = 1;
            setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = INADDR_ANY;
    
            if (bind(listener, (sockaddr*)&addr, sizeof(addr)) < 0) {
                perror("bind");
                close(listener);
                sleep(1); // Подождём немного, чтобы порт освободился
                continue;
            }
    
            if (listen(listener, SOMAXCONN) < 0) {
                perror("listen");
                close(listener);
                sleep(1);
                continue;
            }
    
            int clientSock = accept(listener, nullptr, nullptr);
            if (clientSock < 0) {
                perror("accept");
                close(listener);
                continue;
            }
    
            uint8_t desired;
            if (recv(clientSock, &desired, sizeof(desired), 0) <= 0) {
                std::cout << "⚠️ От клиента получено <= 0 байт.\n";
                close(clientSock);
                close(listener);
                continue;
            }
    
            if (desired == static_cast<uint8_t>(serverFigure)) {
                uint8_t reject = 0xFF;
                std::cout << "⚠️ Фигуры совпадают, отклоняем.\n";
                send(clientSock, &reject, sizeof(reject), 0);
                close(clientSock);
                close(listener);
                continue;
            }
    
            uint8_t response = static_cast<uint8_t>(serverFigure);
            send(clientSock, &response, sizeof(response), 0);
            close(listener); // Закрываем после успешного подключения
            return clientSock;
        }
    }
};