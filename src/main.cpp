#include <SFML/Graphics.hpp>
#include <locale.h>
#include <windows.h>
#include <time.h>

#include <iostream>

#include <interface.hpp>

using namespace std;

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Rus");

    RenderWindow window(VideoMode(600, 640), " рестики-нолики");
    StartWindow startWindow;
    GameField gameField; // «десь 0 - пусто, 1 - крестик, 2 - нолик
    ResultWindow resultWindow;

    bool isMenu = true, isGame = false, isEnd = false;
    bool isBotStep = false, isCursorHand = false, isRandom;
    Result result; // 1 - победа бота, 0 - ничь€, -1 победа игрока
    
    Player player, bot(true);

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
                for (int i = 0; i < 2; i++)
                {
                    if (startWindow.getFigureButtons()[i].sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        isMenu = false;
                        isGame = true;
                        // cursor.loadFromSystem(Cursor::Arrow);
                        if (i == 2)
                        {
                            srand(time(NULL));
                            player.setFigure((Figure) (rand() % 2 + 1));
                            isRandom = true;
                        }
                        else
                        {
                            player.setFigure((Figure) (i + 1));
                            isRandom = false;
                        }
                        if (player.getFigure() == Figure::Cross)
                            bot.setFigure(Figure::Zero);
                        else
                            bot.setFigure(Figure::Cross);

                        gameField.setPlayerFigure(player.getFigure());
                        gameField.setBotFigure(bot.getFigure());

                        if (bot.getFigure() == Figure::Cross) // ≈сли бот - крестик
                        {
                            isBotStep = true;
                            gameField.stepStringHover(Move::BotMove);
                        }
                        else
                        {
                            gameField.stepStringHover(Move::PlayerMove);
                        }
                    }
                }
            }
            else if (Mouse::isButtonPressed(Mouse::Left) && isGame && !isEnd)
            {
                for (int i = 0; i < 9; i++)
                {
                    if ((gameField.getCells()[i].cellSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) && (gameField.getCells()[i].figure == Figure::Empty))
                    {
                        gameField.setCell(i, player.getFigure());
                        if ((result = gameField.checkResult(gameField, gameField.getCells())).winner != NOT_EMPTY)
                        {
                            gameField.changeLinePosition(result);
                            isEnd = true;
                            gameField.stepStringHover(Move::EndGame);
                            break;
                        }
                        isBotStep = true;
                        gameField.stepStringHover(Move::BotMove);
                    }
                }
            }
            else if (Mouse::isButtonPressed(Mouse::Left) && isEnd)
            {
                isEnd = false;
                if (resultWindow.getBackMenuButton().sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    isMenu = true;
                    isRandom = false;
                }
                else
                {
                    if (isRandom)
                    {
                        Figure tempFigure = player.getFigure();
                        player.setFigure(bot.getFigure());
                        bot.setFigure(tempFigure);
                    }
                    isGame = true;
                    if (bot.getFigure() == Figure::Cross) // ≈сли бот - крестик
                    {
                        isBotStep = true;
                        gameField.stepStringHover(Move::BotMove);
                    }
                    else
                    {
                        gameField.stepStringHover(Move::PlayerMove);
                    }
                }
                gameField.clearCells(gameField);
            }
        }

        gameField.updateCells();
        window.clear(Color(20, 189, 172, 255));
        bool newCursor;

        if (isMenu)
        {
            startWindow.figuresHover(mousePos);
            startWindow.mainButtonHover(startWindow.getStartButton(), startWindow.getStartText(), mousePos);
            // app.changeFigureTexture(mousePos, newCursor);
            // if (!newCursor)
            //     app.changeButtonTexture(messages.start, mousePos, newCursor);

            for (int i = 0; i < 3; i++)
                window.draw(startWindow.getAllObjects()[i]->sprite);
            for (int i = 0; i < 2; i++)
                window.draw(*startWindow.getAllTexts()[i]);
        }
        else if (isGame)
        {
            for (int i = 0; i < 9; i++)
                if (gameField.getCells()[i].figure != Figure::Empty)
                    window.draw(gameField.getCells()[i].cellSprite);
            
            for (int i = 0; i < 1; i++)
                window.draw(gameField.getAllObjects()[i]->sprite);
            window.draw(*gameField.getAllTexts()[0]);
            if (isEnd && result.winner != DRAW)
            {
                window.draw(gameField.getLineSprite());
            }
        }
        else if (isEnd)
        {
            int figure = 0;
            // resultWindow.mainButtonHover()
            // app.changeButtonTexture(messages.backMenu, mousePos, newCursor);
            Object *figureButtons = startWindow.getFigureButtons();
            if (result.winner != DRAW)
            {
                Object *figureButtons = startWindow.getFigureButtons();
                (result.winner == PLAYER_WIN) ? figure = ((int) player.getFigure()) - 1 : figure = ((int) bot.getFigure()) - 1;
                figureButtons[figure].sprite.setPosition(200, 100);
                figureButtons[figure].sprite.setTextureRect(IntRect(200 * figure, 0, 200, 200));
                window.draw(figureButtons[figure].sprite);
                figureButtons[figure].sprite.setPosition(50 + 300 * figure, 180);
                // messages.result.setString("ѕќЅ≈ƒ»“≈Ћ№!");
                // messages.result.setPosition(15, 300);
            }
            else
            {
                figureButtons[0].sprite.setPosition(110, 100);
                figureButtons[0].sprite.setTextureRect(IntRect(0, 0, 400, 200));
                window.draw(figureButtons[0].sprite);
                figureButtons[0].sprite.setPosition(50 + 300 * figure, 180);
                // messages.result.setString("Ќ»„№я!");
                // messages.result.setPosition(150, 300);
            }
            window.draw(resultWindow.getAllObjects()[0]->sprite);
            for (int i = 0; i < 2; i++)
                window.draw(*resultWindow.getAllTexts()[i]);
            // window.draw(figureButtons[2].sprite);
            // window.draw(messages.result); 
            // window.draw(messages.backMenu);
        }

        // if (newCursor != isCursorHand && !isCursorHand)
        // {
        //     cursor.loadFromSystem(Cursor::Hand);
        //     isCursorHand = newCursor;
        // }
        // else if (newCursor != isCursorHand && isCursorHand)
        // {
        //     cursor.loadFromSystem(Cursor::Arrow);
        //     isCursorHand = newCursor;
        // }

        // window.setMouseCursor(cursor);
        window.display();

        if (isBotStep)
        {
            Sleep(1000);
            // for (int i = 0; i < 9; i++)
            // {
            //     if (gameField.getCells()[i].figure == Figure::Cross)
            //         cout << "X ";
            //     else if (gameField.getCells()[i].figure == Figure::Zero)
            //         cout << "O ";
            //     else if (gameField.getCells()[i].figure == Figure::Empty)
            //         cout << "E ";
            // }
            // cout << endl;
            Step move = findOptimalMove(gameField, gameField.getCells(), true);
            cout << "SOSIT" << endl;
            isBotStep = false;
            cout << "gotcha " << move.pos << endl;
            gameField.setCell(move.pos, bot.getFigure());
            if ((result = gameField.checkResult(gameField, gameField.getCells())).winner != NOT_EMPTY)
            {
                gameField.changeLinePosition(result);
                gameField.stepStringHover(Move::EndGame);
                isEnd = true;
            }
            else
            {
                gameField.stepStringHover(Move::PlayerMove);
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