#include <SFML/Graphics.hpp>
#include <locale.h>
#include <time.h>
#include <unistd.h>

#include <interface.hpp>
#include <network.hpp>

std::vector<std::string> ipList = {
    "127.0.0.1",
    // "192.168.1.101", // другие IP в сети, если есть
};

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Rus");

    RenderWindow window(VideoMode(600, 640), "Tic-tac-toe");
    StartWindow startWindow;
    GameWindow gameWindow;
    ResultWindow resultWindow;

    bool isBotStep = false;
    Move currentMove;
    Result result;

    Player player, enemy;
    enemy.setGameWindow(&gameWindow);

    MultiplayerManager manager;

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
                    // socket initialization
                    if (startWindow.getAllObjects()[i]->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        if (i == 2)
                        {
                            srand(time(NULL));
                            player.setFigure((Figure)(rand() % 2 + 1));
                        }
                        else
                        {
                            player.setFigure((Figure)(i + 1));
                        }

                        manager.setIpList(ipList);
                        manager.setPort(8787);
                        manager.setMyFigure(player.getFigure());
                        manager.initialize();

                        startWindow.setActive(false);
                        gameWindow.setActive(true);
                        gameWindow.clearCells();
                        gameWindow.setPlayerFigure(player.getFigure());
                        gameWindow.setWaitMode(true);
                        gameWindow.changeStepString(Move::Waiting);

                        sleep(1);

                        break;
                    }
                }
            }
            else if (Mouse::isButtonPressed(Mouse::Left) && gameWindow.isActive() && currentMove == Move::Player)
            {
                for (int i = 0; i < 9; i++)
                {
                    if ((gameWindow.getCells()[i].sprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) && (gameWindow.getCells()[i].figure == Figure::Empty))
                    {
                        gameWindow.setCell(i, player.getFigure());
                        if ((result = gameWindow.checkResult(gameWindow, gameWindow.getCells())).winner != Winner::Unknown)
                        {
                            gameWindow.changeLinePosition(result);
                            gameWindow.changeStepString(Move::EndGame);
                            resultWindow.setActive(true);
                        } else {
                            gameWindow.changeStepString(Move::Enemy);
                            currentMove = Move::Enemy;
                        }

                        // Отправка позиции по сокету
                        uint8_t move = static_cast<uint8_t>(i);
                        send(manager.getSocket(), &move, 1, 0);
                        break;
                    }
                }
            }
            else if (gameWindow.isActive() && currentMove == Move::Enemy && !resultWindow.isActive())
            {
                uint8_t move;
                int bytesRead = recv(manager.getSocket(), &move, 1, MSG_DONTWAIT); // не блокирует цикл

                if (bytesRead == 0) { // Если противник отключился
                    manager.disconnect();
                    gameWindow.clearCells();
                    gameWindow.setActive(false);
                    startWindow.setActive(true);
                    break;
                }

                if (bytesRead == 1 && move < 9)
                {
                    if (gameWindow.getCells()[move].figure == Figure::Empty)
                    {
                        gameWindow.setCell(move, enemy.getFigure());

                        if ((result = gameWindow.checkResult(gameWindow, gameWindow.getCells())).winner != Winner::Unknown)
                        {
                            gameWindow.changeLinePosition(result);
                            gameWindow.changeStepString(Move::EndGame);
                            resultWindow.setActive(true);
                        }
                        else
                        {
                            gameWindow.changeStepString(Move::Player);
                            currentMove = Move::Player;
                        }
                    }
                }
            }
            else if (Mouse::isButtonPressed(Mouse::Left) && resultWindow.isActive())
            {
                if (mousePos.y < 30) {
                    break;
                }
                if (resultWindow.getAllObjects()[0]->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    manager.disconnect();
                    resultWindow.setActive(false);
                    startWindow.setActive(true);
                }
                else if (manager.getSocket() == -1) 
                {
                    manager.initialize();
                    resultWindow.setActive(false);
                    gameWindow.setActive(true);
                    gameWindow.clearCells();
                    gameWindow.setWaitMode(true);
                    gameWindow.changeStepString(Move::Waiting);
                    manager.setReadyToContinue(false);
                    manager.setEnemyReady(false);
                }
                else
                {
                    manager.setReadyToContinue(true);
                    uint8_t msg = 100; // Условный код "готов к новой игре"
                    send(manager.getSocket(), &msg, 1, 0);
                }
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
                (result.winner == Winner::Player) ? figure = ((int)player.getFigure()) - 1 : figure = ((int)enemy.getFigure()) - 1;
                figureButtons[figure]->sprite.setPosition(200, 100);
                figureButtons[figure]->sprite.setTextureRect(IntRect(200 * figure, 0, 200, 200));
                window.draw(figureButtons[figure]->sprite);
                figureButtons[figure]->sprite.setPosition(50 + 300 * figure, 180);
                resultWindow.setBackMenuText("WINNER!");
            }
            else
            {
                figureButtons[0]->sprite.setPosition(110, 100);
                figureButtons[0]->sprite.setTextureRect(IntRect(0, 0, 400, 200));
                window.draw(figureButtons[0]->sprite);
                figureButtons[0]->sprite.setPosition(50 + 300 * figure, 180);
                resultWindow.setBackMenuText("DRAW!");
            }
            
            if (manager.getReadyToContinue() || manager.getEnemyReady()) {
                resultWindow.setWaitText("1/2 players ready...");
            } else if (manager.getSocket() == -1) {
                resultWindow.setWaitText("Opponent disconnected.");
            }

            window.draw(resultWindow.getAllObjects()[0]->sprite);
            for (int i = 0; i < 3; i++)
                window.draw(*resultWindow.getAllTexts()[i]);
            
            window.setMouseCursor(resultWindow.getCursor());
        }

        window.display();

        if (gameWindow.getWaitMode() == true) {
            while (true) {
                if (!manager.isReady()) {
                    continue; // пока ждём подключения
                } else {
                    enemy.setFigure(manager.getOpponentFigure());
                    gameWindow.setWaitMode(false);
                    gameWindow.setEnemyFigure(enemy.getFigure());
                    if (enemy.getFigure() == Figure::Cross)
                    {
                        currentMove = Move::Enemy;
                        gameWindow.changeStepString(currentMove);
                    }
                    else
                    {
                        currentMove = Move::Player;
                        gameWindow.changeStepString(currentMove);
                    }
                    break;
                }
            }
        }

        if (gameWindow.isActive() && resultWindow.isActive())
        {
            sleep(1);
            gameWindow.setActive(false);
        }

        if (resultWindow.isActive() && manager.getSocket() != -1)
        {
            uint8_t buffer;
            int bytesRead = recv(manager.getSocket(), &buffer, 1, MSG_DONTWAIT);

            if (bytesRead == 1 && buffer == 100) {
                manager.setEnemyReady(true);
            }

            if (bytesRead == 0) { // Если противник отключился
                manager.disconnect();
            }

            if (manager.getReadyToContinue() && manager.getEnemyReady()) {
                currentMove = (player.getFigure() == Figure::Cross) ? Move::Player : Move::Enemy;
                gameWindow.clearCells();
                gameWindow.changeStepString(currentMove);
                gameWindow.setActive(true);
                resultWindow.setActive(false);
                manager.setReadyToContinue(false);
                manager.setEnemyReady(false);
                sleep(1);
            }
        }
        usleep(5000);
    }

    return 0;
}