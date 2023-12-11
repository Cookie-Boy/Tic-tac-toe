#include <SFML/Graphics.hpp>
#include <locale.h>
#include <time.h>
#include <windows.h>

#include <interface.hpp>

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Rus");

    RenderWindow window(VideoMode(600, 640), "Крестики-нолики");
    StartWindow startWindow;
    GameWindow gameWindow;
    ResultWindow resultWindow;

    bool isBotStep = false, isRandom;
    Result result;

    Player player, bot;
    bot.setGameWindow(&gameWindow);

    while (window.isOpen())
    {
        Event event;
        Vector2i mousePos = Mouse::getPosition(window);
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            else if (Mouse::isButtonPressed(Mouse::Left) && startWindow.isActive())
            {
                for (int i = 0; i < 3; i++)
                {
                    if (startWindow.getAllObjects()[i]->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        startWindow.setActive(false);
                        gameWindow.setActive(true);

                        if (i == 2)
                        {
                            srand(time(NULL));
                            player.setFigure((Figure)(rand() % 2 + 1));
                            isRandom = true;
                        }
                        else
                        {
                            player.setFigure((Figure)(i + 1));
                            isRandom = false;
                        }

                        if (player.getFigure() == Figure::Cross)
                            bot.setFigure(Figure::Zero);
                        else
                            bot.setFigure(Figure::Cross);

                        gameWindow.setPlayerFigure(player.getFigure());
                        gameWindow.setBotFigure(bot.getFigure());

                        if (bot.getFigure() == Figure::Cross) // Крестики ходят первыми
                        {
                            isBotStep = true;
                            gameWindow.changeStepString(Move::Bot);
                        }
                        else
                        {
                            gameWindow.changeStepString(Move::Player);
                        }
                        break;
                    }
                }
            }
            else if (Mouse::isButtonPressed(Mouse::Left) && gameWindow.isActive())
            {
                for (int i = 0; i < 9; i++)
                {
                    if ((gameWindow.getCells()[i].sprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) && (gameWindow.getCells()[i].figure == Figure::Empty))
                    {
                        gameWindow.setCell(i, player.getFigure());
                        if ((result = gameWindow.checkResult(gameWindow, gameWindow.getCells())).winner != Winner::Unknown)
                        {
                            gameWindow.changeLinePosition(result);
                            resultWindow.setActive(true);
                            gameWindow.changeStepString(Move::EndGame);
                            break;
                        }
                        isBotStep = true;
                        gameWindow.changeStepString(Move::Bot);
                    }
                }
            }
            else if (Mouse::isButtonPressed(Mouse::Left) && resultWindow.isActive())
            {
                resultWindow.setActive(false);
                if (resultWindow.getAllObjects()[0]->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    startWindow.setActive(true);
                    isRandom = false;
                }
                else
                {
                    if (isRandom)
                    {
                        Figure tempFigure = player.getFigure();
                        player.setFigure(bot.getFigure());
                        bot.setFigure(tempFigure);
                        gameWindow.setPlayerFigure(player.getFigure());
                        gameWindow.setBotFigure(bot.getFigure());
                    }

                    gameWindow.setActive(true);
                    if (bot.getFigure() == Figure::Cross) // Крестики ходят первыми
                    {
                        isBotStep = true;
                        gameWindow.changeStepString(Move::Bot);
                    }
                    else
                    {
                        gameWindow.changeStepString(Move::Player);
                    }
                }
                gameWindow.clearCells();
            }
        }

        gameWindow.updateCells();
        window.clear(Color(20, 189, 172, 255));

        if (startWindow.isActive())
        {
            startWindow.hover(startWindow.getAllObjects()[2], startWindow.getStartText(), mousePos);

            for (int i = 0; i < 3; i++)
                window.draw(startWindow.getAllObjects()[i]->sprite);
            for (int i = 0; i < 2; i++)
                window.draw(*startWindow.getAllTexts()[i]);
            window.setMouseCursor(startWindow.getCursor());
        }
        else if (gameWindow.isActive())
        {
            for (int i = 0; i < 9; i++)
                if (gameWindow.getCells()[i].figure != Figure::Empty)
                    window.draw(gameWindow.getCells()[i].sprite);

            for (int i = 0; i < 1; i++)
                window.draw(gameWindow.getAllObjects()[i]->sprite);
            window.draw(*gameWindow.getAllTexts()[0]);
            if (resultWindow.isActive() && result.winner != Winner::Draw)
            {
                window.draw(gameWindow.getLineSprite());
            }
            window.setMouseCursor(gameWindow.getCursor());
        }
        else if (resultWindow.isActive())
        {
            int figure = 0;
            resultWindow.hover(resultWindow.getAllObjects()[0], resultWindow.getBackMenuText(), mousePos);
            Object **figureButtons = startWindow.getAllObjects();
            if (result.winner != Winner::Draw)
            {
                (result.winner == Winner::Player) ? figure = ((int)player.getFigure()) - 1 : figure = ((int)bot.getFigure()) - 1;
                figureButtons[figure]->sprite.setPosition(200, 100);
                figureButtons[figure]->sprite.setTextureRect(IntRect(200 * figure, 0, 200, 200));
                window.draw(figureButtons[figure]->sprite);
                figureButtons[figure]->sprite.setPosition(50 + 300 * figure, 180);
                resultWindow.setBackMenuText("ПОБЕДИТЕЛЬ!");
            }
            else
            {
                figureButtons[0]->sprite.setPosition(110, 100);
                figureButtons[0]->sprite.setTextureRect(IntRect(0, 0, 400, 200));
                window.draw(figureButtons[0]->sprite);
                figureButtons[0]->sprite.setPosition(50 + 300 * figure, 180);
                resultWindow.setBackMenuText("НИЧЬЯ!");
            }
            window.draw(resultWindow.getAllObjects()[0]->sprite);
            for (int i = 0; i < 2; i++)
                window.draw(*resultWindow.getAllTexts()[i]);
            window.setMouseCursor(resultWindow.getCursor());
        }

        window.display();

        if (isBotStep)
        {
            Sleep(1000);
            Step move = bot.findOptimalMove(gameWindow.getCells(), true);
            isBotStep = false;
            gameWindow.setCell(move.pos, bot.getFigure());
            if ((result = gameWindow.checkResult(gameWindow, gameWindow.getCells())).winner != Winner::Unknown)
            {
                gameWindow.changeLinePosition(result);
                gameWindow.changeStepString(Move::EndGame);
                resultWindow.setActive(true);
            }
            else
            {
                gameWindow.changeStepString(Move::Player);
            }
        }
        else if (gameWindow.isActive() && resultWindow.isActive())
        {
            Sleep(1000);
            gameWindow.setActive(false);
        }
    }

    return 0;
}