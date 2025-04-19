#include "name_window.hpp"

NameInputWindow::NameInputWindow() {
    font.loadFromFile("utils/RobotoSerif.ttf");

    prompt.setFont(font);
    prompt.setString("Enter your name:");
    prompt.setCharacterSize(30);
    prompt.setPosition(100, 100);
    prompt.setFillColor(sf::Color::White);

    inputText.setFont(font);
    inputText.setCharacterSize(28);
    inputText.setPosition(100, 160);
    inputText.setFillColor(sf::Color::Black);

    inputBox.setPosition(90, 155);
    inputBox.setSize(sf::Vector2f(400, 40));
    inputBox.setFillColor(sf::Color(200, 200, 200));
}

void NameInputWindow::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == '\b' && !playerName.empty()) {
            playerName.pop_back();
        } else if (event.text.unicode == '\r') {
            finished = true;
        } else if (event.text.unicode < 128 && playerName.size() < 20) {
            playerName += static_cast<char>(event.text.unicode);
        }
        inputText.setString(playerName);
    }
}

void NameInputWindow::render(sf::RenderWindow& window) {
    window.clear(sf::Color(20, 189, 172, 255));
    window.draw(inputBox);
    window.draw(prompt);
    window.draw(inputText);
}