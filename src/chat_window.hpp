#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#define SERVER_PORT 9000

class ChatWindow {
public:
    ChatWindow();

    void startNetwork(const std::string& serverIp, int port);
    void handleEvent(const sf::Event& event);
    void update();
    void render(sf::RenderWindow& window);

    bool isActive();
    void setActive(bool active);

    void setPlayerName(std::string playerName);
    void shutdownConnection();

private:
    void receiveLoop();
    void appendMessage(const std::string& message);

    sf::Font font_;
    sf::Text inputText_;
    sf::Text messagesText_;
    sf::RectangleShape inputBox_;
    sf::RectangleShape closeButton_;

    std::string currentInput_;
    std::string playerName_;

    std::vector<std::string> messages_;
    std::mutex mutex_;

    int sockfd_ = -1;
    bool running_ = false;
    bool active_ = false;
    bool justActivated_ = false;
    std::thread receiveThread_;
    std::thread shutdownThread_;
    std::atomic<bool> shutdownTimerRunning_{false};
};
