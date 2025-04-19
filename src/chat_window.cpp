#include "chat_window.hpp"

ChatWindow::ChatWindow() {
    font_.loadFromFile("utils/RobotoSerif.ttf");

    inputText_.setFont(font_);
    inputText_.setCharacterSize(20);
    inputText_.setFillColor(sf::Color::Black);
    inputText_.setPosition(10, 550);

    messagesText_.setFont(font_);
    messagesText_.setCharacterSize(18);
    messagesText_.setFillColor(sf::Color::White);
    messagesText_.setPosition(10, 10);

    inputBox_.setPosition(0, 545);
    inputBox_.setSize(sf::Vector2f(600, 40));
    inputBox_.setFillColor(sf::Color(200, 200, 200));

    closeButton_.setSize({20, 20});
    closeButton_.setPosition(580, 0);
    closeButton_.setFillColor(sf::Color::Red);
}

void ChatWindow::startNetwork(const std::string& serverIp, int port) {
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_ < 0) {
        std::cerr << "Socket creation error\n";
        return;
    }
    
    sockaddr_in servAddr{};
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    inet_pton(AF_INET, serverIp.c_str(), &servAddr.sin_addr);
    
    if (connect(sockfd_, (sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        std::cerr << "Can not connect to the server\n";
        close(sockfd_);
        sockfd_ = -1;
        return;
    }
    
    running_ = true;
    receiveThread_ = std::thread(&ChatWindow::receiveLoop, this);
    receiveThread_.detach();
}

void ChatWindow::receiveLoop() {
    char buffer[1024];
    while (running_) {
        ssize_t received = recv(sockfd_, buffer, sizeof(buffer) - 1, MSG_DONTWAIT);
        if (received > 0) {
            std::cout << "received > 0" << std::endl;
            buffer[received] = '\0';
            std::lock_guard<std::mutex> lock(mutex_);
            messages_.emplace_back(buffer);
            if (messages_.size() > 20)
                messages_.erase(messages_.begin());
        } else if (received == 0) {
            std::cout << "Connection is closed" << std::endl;
            break;
        } else {
            if (errno != EWOULDBLOCK && errno != EAGAIN) {
                std::cout << "Some error happened." << std::endl;
                break;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


void ChatWindow::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        if (justActivated_) {
            justActivated_ = false;
            return;
        }

        if (event.text.unicode == '\b' && !currentInput_.empty()) {
            currentInput_.pop_back();
        } else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
            if (!currentInput_.empty() && running_) {
                std::string fullMessage = playerName_ + ": " + currentInput_;
                send(sockfd_, fullMessage.c_str(), fullMessage.size(), 0);
                appendMessage(fullMessage);
                currentInput_.clear();
            }
        } else if (event.text.unicode < 128) {
            currentInput_ += static_cast<char>(event.text.unicode);
        }
    } else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);
            if (closeButton_.getGlobalBounds().contains(clickPos.x, clickPos.y)) {
                setActive(false);
            }
        }
    }
}

void ChatWindow::appendMessage(const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    messages_.emplace_back(message);
    if (messages_.size() > 20)
        messages_.erase(messages_.begin());
}

void ChatWindow::update() {
    inputText_.setString(currentInput_);

    std::string allMessages;
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& msg : messages_) {
        allMessages += msg + "\n";
    }
    messagesText_.setString(allMessages);
}

void ChatWindow::render(sf::RenderWindow& window) {
    update();
    window.draw(messagesText_);
    window.draw(inputBox_);
    window.draw(inputText_);
    window.draw(closeButton_);
}

bool ChatWindow::isActive() {
    return active_;
}

void ChatWindow::setActive(bool active) {
    if (active) {
        if (shutdownTimerRunning_) {
            shutdownTimerRunning_ = false;
        }
        
        if (sockfd_ == -1) {
            startNetwork("127.0.0.1", SERVER_PORT);
        }
        
        active_ = true;
        justActivated_ = true;
        return;
    }

    if (!shutdownTimerRunning_) {
        shutdownTimerRunning_ = true;
        shutdownThread_ = std::thread([this]() {
            for (int i = 0; i < 5 && shutdownTimerRunning_; i++) {
                std::cout << "Waiting for return: " << i + 1 << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }

            if (shutdownTimerRunning_) {
                shutdownConnection();
            }
        });
        shutdownThread_.detach();
    }
    this->active_ = false;
}

void ChatWindow::shutdownConnection() {
    running_ = false;

    if (sockfd_ != -1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Shuwdown sockfd: " << sockfd_ << std::endl;
        shutdown(sockfd_, SHUT_RDWR);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        close(sockfd_);
        sockfd_ = -1;
    }

    messages_.clear();
    currentInput_.clear();
    shutdownTimerRunning_ = false;
}

void ChatWindow::setPlayerName(std::string playerName) {
    this->playerName_ = playerName;
}