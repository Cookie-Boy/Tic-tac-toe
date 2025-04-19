#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class NameInputWindow {
public:
    NameInputWindow();
    void handleEvent(const sf::Event& event);
    void render(sf::RenderWindow& window);
    std::string getPlayerName() const { return playerName; }
    bool isFinished() const { return finished; }

private:
    sf::Font font;
    sf::Text prompt;
    sf::Text inputText;
    std::string playerName;
    sf::RectangleShape inputBox;
    bool finished = false;
};
