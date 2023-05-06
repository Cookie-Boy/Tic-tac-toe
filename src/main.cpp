#include <SFML/Graphics.hpp>
#include <iostream>
#include <locale.h>
#include <windows.h>

#include <environment.hpp>

using namespace std;

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Rus");
    RenderWindow window(VideoMode(600, 640), "Крестики-нолики"); // Создаем окно для игры

    Texture figures;
    figures.loadFromFile("img/figures.png");
    Sprite choice[2];
    for (int i = 0; i < 2; i++)
    {
        choice[i].setTexture(figures);
        choice[i].setPosition(50 + 300 * i, 220);
    }

    Texture backgroundTexture;
    backgroundTexture.loadFromFile("img/background.png");
    Sprite background(backgroundTexture);

    Font font;
    font.loadFromFile("utils/countryhouse.ttf");
    Text mainMessage("Выбери фигуру:", font, 80);
    mainMessage.setFillColor(Color::Black);
    mainMessage.setStyle(Text::Bold);
    mainMessage.setPosition(65, 50);

    Text stepMessage("Твой ход!", font, 30);
    stepMessage.setFillColor(Color::Green);
    stepMessage.setStyle(Text::Bold);
    stepMessage.setPosition(250, 598);

    Texture lineTexture;
    lineTexture.loadFromFile("img/line.png");
    Sprite line(lineTexture);

    bool onMenu = true, onGame = false, isBotStep = false, isEnd = false;
    int playerCharacter = 0, botCharacter = 0; // 0 - крестик, 1 - нолик
    Init player(figures), bot(figures);

    while (window.isOpen())
    {
        Vector2i mousePosition = Mouse::getPosition(window);
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (Mouse::isButtonPressed(Mouse::Left))
            {
                if (onMenu)
                {
                    for (int i = 0; i < 2; i++)
                    {
                        if (choice[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
                        {
                            playerCharacter = i;
                            botCharacter = !playerCharacter;
                            onMenu = false;
                            onGame = true;
                        }
                    }
                }
                else if (onGame && !isEnd)
                {
                    for (int i = 0; i < 9; i++)
                    {
                        if ((player.cells[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y)) && (!player.cellMode[i] && !bot.cellMode[i]))
                        {
                            player.cellMode[i] = true;
                            if (checkResult(player, bot, line))
                            {
                                isEnd = true;
                                break;
                            }
                            isBotStep = true;
                            stepMessage.setFillColor(Color::Red);
                            stepMessage.setString("Ход бота");
                        }
                    }
                }
                else if (isEnd)
                {
                    isEnd = false;
                    onGame = false;
                    onMenu = true;
                    clearCells(player, bot);
                }
            }
        }

        hover(choice, mousePosition);

        player.update(playerCharacter);
        bot.update(botCharacter);

        window.clear(Color::White);

        if (onMenu)
        {
            for (int i = 0; i < 2; i++)
                window.draw(choice[i]);
            window.draw(mainMessage);
        }
        else if (onGame)
        {
            for (int i = 0; i < 9; i++)
                if (player.cellMode[i])
                    window.draw(player.cells[i]);
                else if (bot.cellMode[i])
                    window.draw(bot.cells[i]);

            window.draw(stepMessage);
            window.draw(background);

            if (isEnd)
                window.draw(line);
        }

        window.display();

        if (isBotStep)
        {
            Sleep(1000);
            isBotStep = false;
            smartStep(player, bot);
            if (checkResult(player, bot, line))
            {
                isEnd = true;
            }
            stepMessage.setFillColor(Color::Green);
            stepMessage.setString("Твой ход!");
        }
    }
    
    return 0;
}