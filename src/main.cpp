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
        choice[i].setPosition(50 + 300 * i, 180);
    }

    Texture backgroundTexture;
    backgroundTexture.loadFromFile("img/background.png");
    Sprite background(backgroundTexture);

    Font font;
    font.loadFromFile("utils/RobotoSerif.ttf");

    Texture lineTexture;
    lineTexture.loadFromFile("img/line.png");
    Sprite line(lineTexture);

    Texture startButtonTexture;
    startButtonTexture.loadFromFile("img/button.png");
    Sprite startButton(startButtonTexture);
    startButton.setPosition(125, 475);

    bool onMenu = true, onGame = false, isBotStep = false, isEnd = false;
    bool isCursorHand = false;
    int playerCharacter = 0, botCharacter = 0; // 0 - крестик, 1 - нолик
    int result; // 1 - победа игрока, 2 - победа бота, 3 - ничья
    Init player(figures), bot(figures);

    Text mainMessage("Выбери фигуру", font, 60);
    mainMessage.setFillColor(Color::Black);
    mainMessage.setStyle(Text::Bold);
    mainMessage.setPosition(60, 50);

    Text stepMessage("", font, 30);
    stepMessage.setStyle(Text::Bold);

    Text winMessage("", font, 75);
    winMessage.setFillColor(Color::Black);
    winMessage.setStyle(Text::Bold);

    Text startGameMessage("НАЧАТЬ ИГРУ", font, 43);
    startGameMessage.setStyle(Text::Bold);
    startGameMessage.setPosition(135, 485);

    Text mainMenuMessage("ГЛАВНОЕ МЕНЮ", font, 37);
    mainMenuMessage.setStyle(Text::Bold);
    mainMenuMessage.setPosition(130, 490);

    Cursor cursor;
    cursor.loadFromSystem(Cursor::Arrow);

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
                            cursor.loadFromSystem(Cursor::Arrow);
                            playerCharacter = i;
                            botCharacter = !playerCharacter;
                            onMenu = false;
                            onGame = true;
                            if (!botCharacter) // Если бот - крестик
                            {
                                isBotStep = true;
                                changeStepString(stepMessage, botCharacter);
                            }
                            else
                            {
                                changeStepString(stepMessage, playerCharacter);
                            }
                        }
                        else if (startButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
                        {
                            cursor.loadFromSystem(Cursor::Arrow);
                            putRandomValues(playerCharacter, botCharacter);
                            onMenu = false;
                            onGame = true;
                            if (!botCharacter) // Если бот - крестик
                            {
                                isBotStep = true;
                                changeStepString(stepMessage, botCharacter);
                            }
                            else
                            {
                                changeStepString(stepMessage, playerCharacter);
                            }
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
                            if (result = checkResult(player, bot, line))
                            {
                                isEnd = true;
                                changeStepString(stepMessage, 2);
                                break;
                            }
                            isBotStep = true;
                            changeStepString(stepMessage, botCharacter);
                        }
                    }
                }
                else if (isEnd)
                {
                    isEnd = false;
                    // Sleep(350);
                    if (startButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
                        onMenu = true;
                    else
                        onGame = true;

                    if (!botCharacter) // Если бот - крестик
                    {
                        isBotStep = true;
                        changeStepString(stepMessage, botCharacter);
                    }
                    else
                    {
                        changeStepString(stepMessage, playerCharacter);
                    }
                    clearCells(player, bot);
                }
            }
        }

        bool newCursor;
        if (onMenu)
        {
            changeFigureTexture(choice, mousePosition, newCursor);
            if (!newCursor)
                changeButtonTexture(startButton, startGameMessage, mousePosition, newCursor);
        }
        else if (isEnd)
        {
            changeButtonTexture(startButton, mainMenuMessage, mousePosition, newCursor);
        }
            
        player.update(playerCharacter);
        bot.update(botCharacter);

        window.clear(Color(20, 189, 172, 255));

        if (newCursor != isCursorHand && !isCursorHand)
        {
            cursor.loadFromSystem(Cursor::Hand);
            isCursorHand = newCursor;
        }
        else if (newCursor != isCursorHand && isCursorHand)
        {
            cursor.loadFromSystem(Cursor::Arrow);
            isCursorHand = newCursor;
        }
        window.setMouseCursor(cursor);

        if (onMenu)
        {
            for (int i = 0; i < 2; i++)
                window.draw(choice[i]);
            window.draw(mainMessage);
            window.draw(startButton);
            window.draw(startGameMessage);
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
            if (isEnd && result != DRAW)
                window.draw(line);
        }
        else if (isEnd)
        {
            int figure;
            if (result != DRAW)
            {
                if (result == PLAYER_WIN)
                {
                    figure = playerCharacter;
                }
                else
                {
                    figure = botCharacter;
                }
                choice[figure].setPosition(200, 100);
                choice[figure].setTextureRect(IntRect(200 * figure, 0, 200, 200));
                window.draw(choice[figure]);
                choice[figure].setPosition(50 + 300 * figure, 180);
                winMessage.setString("ПОБЕДИТЕЛЬ!");
                winMessage.setPosition(15, 300);
            }
            else
            {
                choice[0].setPosition(110, 100);
                choice[0].setTextureRect(IntRect(0, 0, 400, 200));
                window.draw(choice[0]);
                choice[0].setPosition(50 + 300 * figure, 180);
                winMessage.setString("НИЧЬЯ!");
                winMessage.setPosition(150, 300);
            }
            window.draw(winMessage); 
            window.draw(startButton);
            window.draw(mainMenuMessage);
        }
        window.display();

        if (isBotStep)
        {
            Sleep(1000);
            isBotStep = false;
            makeSmartMove(player, bot);
            if (result = checkResult(player, bot, line))
            {
                changeStepString(stepMessage, 2);
                isEnd = true;
            }
            else
            {
                changeStepString(stepMessage, playerCharacter);
            }
        }
        else if (onGame && isEnd)
        {
            Sleep(1000);
            onGame = false;
        }
    }
    
    return 0;
}