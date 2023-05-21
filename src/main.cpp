#include <SFML/Graphics.hpp>
#include <locale.h>
#include <windows.h>
#include <time.h>

#include <definitions.hpp>
#include <interface.hpp>

int playerCharacter, botCharacter;

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Rus");
    Font font;
    Cursor cursor;
    Widgets widgets;
    Texts messages;
    RenderWindow window(VideoMode(600, 640), " рестики-нолики"); // —оздаем окно дл€ игры

    createAllWidgets(widgets);
    createAllTextWidgets(font, messages);

    bool isMenu = true, isGame = false, isEnd = false;
    bool isBotStep = false, isCursorHand = false, isRandom;
    Result result; // 1 - победа бота, 0 - ничь€, -1 победа игрока
    int gameField[9] = { 0 }; // «десь 0 - пусто, 1 - крестик, 2 - нолик
    Init player(widgets.main[0].texture), bot(widgets.main[0].texture);

    while (window.isOpen())
    {
        Event event;
        Vector2i mousePos = Mouse::getPosition(window);
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            else if (Mouse::isButtonPressed(Mouse::Left) && isMenu)
            {
                for (int i = 0; i < 3; i++)
                {
                    if (widgets.main[i].sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        isMenu = false;
                        isGame = true;
                        cursor.loadFromSystem(Cursor::Arrow);
                        if (i == 2)
                        {
                            srand(time(NULL));
                            playerCharacter = rand() % 2 + 1;
                            isRandom = true;
                        }
                        else
                        {
                            playerCharacter = i + 1;
                            isRandom = false;
                        }
                        (playerCharacter == 1) ? botCharacter = 2 : botCharacter = 1;

                        if (botCharacter == 1) // ≈сли бот - крестик
                        {
                            isBotStep = true;
                            changeStepString(messages.step, BOT_MOVE);
                        }
                        else
                        {
                            changeStepString(messages.step, PLAYER_MOVE);
                        }
                    }
                }
            }
            else if (Mouse::isButtonPressed(Mouse::Left) && isGame && !isEnd)
            {
                for (int i = 0; i < 9; i++)
                {
                    if ((player.cells[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) && (!gameField[i] && !gameField[i]))
                    {
                        gameField[i] = playerCharacter;
                        if ((result = checkResult(gameField)).winner != NOT_EMPTY)
                        {
                            changeLinePosition(result, widgets.line.sprite);
                            isEnd = true;
                            changeStepString(messages.step, END_GAME);
                            break;
                        }
                        isBotStep = true;
                        changeStepString(messages.step, BOT_MOVE);
                    }
                }
            }
            else if (Mouse::isButtonPressed(Mouse::Left) && isEnd)
            {
                isEnd = false;
                if (widgets.main[2].sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    isMenu = true;
                    isRandom = false;
                }
                else
                {
                    if (isRandom)
                    {
                        int tempCharacter = playerCharacter;
                        playerCharacter = botCharacter;
                        botCharacter = tempCharacter;
                    }
                    isGame = true;
                    if (botCharacter == 1) // ≈сли бот - крестик
                    {
                        isBotStep = true;
                        changeStepString(messages.step, BOT_MOVE);
                    }
                    else
                    {
                        changeStepString(messages.step, PLAYER_MOVE);
                    }
                }
                clearCells(gameField);
            }
        }

        player.update(playerCharacter);
        bot.update(botCharacter);
        window.clear(Color(20, 189, 172, 255));
        bool newCursor;

        if (isMenu)
        {
            changeFigureTexture(widgets, mousePos, newCursor);
            if (!newCursor)
                changeButtonTexture(widgets, messages.start, mousePos, newCursor);
            for (int i = 0; i < 3; i++)
                    window.draw(widgets.main[i].sprite);
            window.draw(messages.header);
            window.draw(messages.start);
        }
        else if (isGame)
        {
            for (int i = 0; i < 9; i++)
                if (gameField[i] == playerCharacter)
                    window.draw(player.cells[i]);
                else if (gameField[i] == botCharacter)
                    window.draw(bot.cells[i]);

            window.draw(messages.step);
            window.draw(widgets.background.sprite);
            if (isEnd && result.winner != DRAW)
                window.draw(widgets.line.sprite);
        }
        else if (isEnd)
        {
            int figure = 0;
            changeButtonTexture(widgets, messages.backMenu, mousePos, newCursor);
            if (result.winner != DRAW)
            {
                (result.winner == PLAYER_WIN) ? figure = playerCharacter - 1 : figure = botCharacter - 1;
                widgets.main[figure].sprite.setPosition(200, 100);
                widgets.main[figure].sprite.setTextureRect(IntRect(200 * figure, 0, 200, 200));
                window.draw(widgets.main[figure].sprite);
                widgets.main[figure].sprite.setPosition(50 + 300 * figure, 180);
                messages.result.setString("ѕќЅ≈ƒ»“≈Ћ№!");
                messages.result.setPosition(15, 300);
            }
            else
            {
                widgets.main[0].sprite.setPosition(110, 100);
                widgets.main[0].sprite.setTextureRect(IntRect(0, 0, 400, 200));
                window.draw(widgets.main[0].sprite);
                widgets.main[0].sprite.setPosition(50 + 300 * figure, 180);
                messages.result.setString("Ќ»„№я!");
                messages.result.setPosition(150, 300);
            }
            window.draw(widgets.main[2].sprite);
            window.draw(messages.result); 
            window.draw(messages.backMenu);
        }

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
        window.display();

        if (isBotStep)
        {
            Sleep(1000);
            isBotStep = false;
            Step move = findOptimalMove(gameField, botCharacter);
            gameField[move.pos] = botCharacter;
            if ((result = checkResult(gameField)).winner != NOT_EMPTY)
            {
                changeLinePosition(result, widgets.line.sprite);
                changeStepString(messages.step, END_GAME);
                isEnd = true;
            }
            else
            {
                changeStepString(messages.step, PLAYER_MOVE);
            }
        }
        else if (isGame && isEnd)
        {
            Sleep(1000);
            isGame = false;
        }
    }

    return 0;
}