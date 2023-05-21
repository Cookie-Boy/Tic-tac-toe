#include <SFML/Graphics.hpp>
#include <iostream>
#include <locale.h>
#include <windows.h>

#include <environment.hpp>

using namespace std;

int playerCharacter, botCharacter;

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Rus");
    Font font;
    Cursor cursor;
    Texture figures, backgroundTexture, startButtonTexture, lineTexture;
    Sprite choice[3], background, line;
    Text mainMessage, stepMessage, winMessage, startGameMessage, mainMenuMessage;
    RenderWindow window(VideoMode(600, 640), " рестики-нолики"); // —оздаем окно дл€ игры

    createAllWidgets(figures, startButtonTexture, choice, backgroundTexture, background, lineTexture, line);
    createAllTextWidgets(font, mainMessage, stepMessage, winMessage, startGameMessage, mainMenuMessage);

    bool onMenu = true, onGame = false, isBotStep = false, isEnd = false;
    bool isCursorHand = false;
    int result; // 1 - победа игрока, 2 - победа бота, 3 - ничь€
    int gameField[9] = { 0 }; // «десь 0 - пусто, 1 - крестик, 2 - нолик
    Init player(figures), bot(figures);

    cursor.loadFromSystem(Cursor::Arrow);

    while (window.isOpen())
    {
        Vector2i mousePosition = Mouse::getPosition(window);
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (Mouse::isButtonPressed(Mouse::Left) && onMenu)
            {
                for (int i = 0; i < 3; i++)
                {
                    if (choice[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
                    {
                        onMenu = false;
                        onGame = true;
                        cursor.loadFromSystem(Cursor::Arrow);
                        if (i == 2)
                        {
                            putRandomValues();
                        }
                        else
                        {
                            playerCharacter = i + 1;
                            (playerCharacter == 1) ? botCharacter = 2 : botCharacter = 1;
                        }

                        if (botCharacter == 1) // ≈сли бот - крестик
                        {
                            isBotStep = true;
                            changeStepString(stepMessage, BOT_MOVE);
                        }
                        else
                        {
                            changeStepString(stepMessage, PLAYER_MOVE);
                        }
                    }
                }
            }
            else if (Mouse::isButtonPressed(Mouse::Left) && onGame && !isEnd)
            {
                for (int i = 0; i < 9; i++)
                {
                    if ((player.cells[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y)) && (!gameField[i] && !gameField[i]))
                    {
                        gameField[i] = playerCharacter;
                        if (result = checkResult(gameField, line))
                        {
                            isEnd = true;
                            changeStepString(stepMessage, END_GAME);
                            break;
                        }
                        isBotStep = true;
                        changeStepString(stepMessage, BOT_MOVE);
                    }
                }
            }
            else if (Mouse::isButtonPressed(Mouse::Left) && isEnd)
            {
                isEnd = false;
                // Sleep(350);
                if (choice[2].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
                {
                    onMenu = true;
                }
                else
                {
                    onGame = true;
                    if (botCharacter == 1) // ≈сли бот - крестик
                    {
                        isBotStep = true;
                        changeStepString(stepMessage, BOT_MOVE);
                    }
                    else
                    {
                        changeStepString(stepMessage, PLAYER_MOVE);
                    }
                }
                clearCells(gameField);
            }
        }

        bool newCursor;
        if (onMenu)
        {
            changeFigureTexture(choice, mousePosition, newCursor);
            if (!newCursor)
                changeButtonTexture(choice[2], startGameMessage, mousePosition, newCursor);
        }
        else if (isEnd)
        {
            changeButtonTexture(choice[2], mainMenuMessage, mousePosition, newCursor);
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
            window.draw(choice[2]);
            window.draw(startGameMessage);
        }
        else if (onGame)
        {
            for (int i = 0; i < 9; i++)
                if (gameField[i] == playerCharacter)
                    window.draw(player.cells[i]);
                else if (gameField[i] == botCharacter)
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
                    figure = playerCharacter - 1;
                }
                else
                {
                    figure = botCharacter - 1;
                }
                choice[figure].setPosition(200, 100);
                choice[figure].setTextureRect(IntRect(200 * figure, 0, 200, 200));
                window.draw(choice[figure]);
                choice[figure].setPosition(50 + 300 * figure, 180);
                winMessage.setString("ѕќЅ≈ƒ»“≈Ћ№!");
                winMessage.setPosition(15, 300);
            }
            else
            {
                choice[0].setPosition(110, 100);
                choice[0].setTextureRect(IntRect(0, 0, 400, 200));
                window.draw(choice[0]);
                choice[0].setPosition(50 + 300 * figure, 180);
                winMessage.setString("Ќ»„№я!");
                winMessage.setPosition(150, 300);
            }
            window.draw(winMessage); 
            window.draw(choice[2]);
            window.draw(mainMenuMessage);
        }
        window.display();

        if (isBotStep)
        {
            Sleep(1000);
            isBotStep = false;
            Step move = findOptimalMove(gameField, botCharacter, line);
            gameField[move.pos] = botCharacter;
            if (result = checkResult(gameField, line))
            {
                changeStepString(stepMessage, END_GAME);
                isEnd = true;
            }
            else
            {
                changeStepString(stepMessage, PLAYER_MOVE);
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