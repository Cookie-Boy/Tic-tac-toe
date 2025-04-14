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

#define PORT 8787

class NetworkManager {
public:
    int getSocket() const;
    void setMyFigure(Figure myFigure);
    Figure getOpponentFigure() const;
    bool isReady() const;
    void setIpList(std::vector<std::string> ipList);
    void setPort(int port);
    bool getReadyToContinue();
    void setReadyToContinue(bool readyToContinue);
    bool getEnemyReady();
    void setEnemyReady(bool enemyReady);
    void initialize();
    void disconnect();

private:
    std::vector<std::string> ipList;
    int port;

    int sockfd = -1;
    Figure myFigure;
    Figure opponentFigure;

    std::atomic<bool> ready = false;
    std::thread serverThread;

    bool readyToContinue = false;
    bool enemyReady;

    bool tryConnect(const char* ip, int port, int& outSock, Figure myFigure, Figure& serverFigure);
    int waitForClient(int port, Figure serverFigure);
};