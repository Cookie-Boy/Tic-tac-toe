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

private:
    std::vector<std::string> ipList;
    int port;

    int sockfd = -1;
    bool isServer = false;
    Figure myFigure;
    Figure opponentFigure;

    std::atomic<bool> ready = false;
    std::thread serverThread;

    bool tryConnect(const char* ip, int port, int& outSock, Figure myFigure, Figure& serverFigure) {
        int s = socket(AF_INET, SOCK_STREAM, 0);
        if (s < 0) return false;

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, ip, &addr.sin_addr);

        if (connect(s, (sockaddr*)&addr, sizeof(addr)) < 0) {
            close(s);
            return false;
        }

        // Отправляем желаемую фигуру
        uint8_t desired = static_cast<uint8_t>(myFigure);
        send(s, &desired, sizeof(desired), 0);

        // Получаем ответ (фигура сервера или отказ)
        uint8_t response;
        if (recv(s, &response, sizeof(response), 0) <= 0 || response == 0xFF) {
            close(s);
            return false;
        }

        serverFigure = static_cast<Figure>(response);
        outSock = s;
        return true;
    }

    int waitForClient(int port, Figure serverFigure) {
        int listener = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;

        bind(listener, (sockaddr*)&addr, sizeof(addr));
        listen(listener, 1);

        int clientSock = accept(listener, nullptr, nullptr);
        close(listener); // Принимаем только одного

        // Получаем желаемую фигуру от клиента
        uint8_t desired;
        if (recv(clientSock, &desired, sizeof(desired), 0) <= 0) {
            close(clientSock);
            return -1;
        }

        if (desired == static_cast<uint8_t>(serverFigure)) {
            uint8_t reject = 0xFF;
            send(clientSock, &reject, sizeof(reject), 0);
            close(clientSock);
            return -1;
        }

        uint8_t response = static_cast<uint8_t>(serverFigure);
        send(clientSock, &response, sizeof(response), 0);
        return clientSock;
    }
};
