#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include <locale.h>
#include <windows.h>

#define PLAYER_WON 1
#define BOT_WON 2

using namespace sf;
using namespace std;

class Init
{
public:
    Sprite cells[9]; // Всего 9 клеток
    bool cellMode[9];

    Init(Texture &image)
    {
        for (int i = 0; i < 9; i++)
        {
            cells[i].setTexture(image);
            cellMode[i] = false;
        }
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                cells[i * 3 + j].setPosition(200 * j, 200 * i);
    }

    void update(int &figure)
    {
        for (int i = 0; i < 9; i++)
            cells[i].setTextureRect(IntRect(200 * figure, 0, 200, 200));
    }
};

void displayButtonMode(Sprite *choice, Vector2i pos)
{
    for (int i = 0; i < 2; i++)
    {
        if (choice[i].getGlobalBounds().contains(pos.x, pos.y))
            choice[i].setTextureRect(IntRect(200 * i, 200, 200, 200));
        else
            choice[i].setTextureRect(IntRect(200 * i, 0, 200, 200));
    }
}

void smartStep(Init &player, Init &bot)
{
    int random;
    do
    {
        random = rand() % 9;
    } while (player.cellMode[random] || bot.cellMode[random]);
    
    bot.cellMode[random] = true;
}

int checkHorizontal(Init &player, Init &bot, Sprite &line)
{
    int playerCounter = 0, botCounter = 0;
    for (int i = 0; i < 7; i += 3)
    {
        for (int j = i; j < (i + 3); j++)
        {
            if (player.cellMode[j])
                playerCounter++;
            else if (bot.cellMode[j])
                botCounter++;

            if (playerCounter == 3)
            {
                line.setTextureRect(IntRect(0, 0, 600, 10));
                line.setRotation(0);
                int distance = ((i / 3) * 200) + 100;
                line.setPosition(0, distance);
                return PLAYER_WON;
            }
            else if (botCounter == 3)
            {
                line.setTextureRect(IntRect(0, 0, 600, 10));
                line.setRotation(0);
                int distance = ((i / 3) * 200) + 100;
                line.setPosition(0, distance);
                return BOT_WON;
            }
        }
        playerCounter = 0;
        botCounter = 0;
    }
    return 0;
}

int checkVertical(Init &player, Init &bot, Sprite &line)
{
    int playerCounter = 0, botCounter = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = i; j < (i + 7); j += 3)
        {
            if (player.cellMode[j])
                playerCounter++;
            else if (bot.cellMode[j])
                botCounter++;

            if (playerCounter == 3)
            {
                line.setTextureRect(IntRect(0, 0, 600, 10));
                line.setRotation(90);
                int distance = (i * 200) + 105;
                line.setPosition(distance, 0);
                return PLAYER_WON;
            }
            else if (botCounter == 3)
            {
                line.setTextureRect(IntRect(0, 0, 600, 10));
                line.setRotation(90);
                int distance = (i * 200) + 105;
                line.setPosition(distance, 0);
                return BOT_WON;
            }
        }
        playerCounter = 0;
        botCounter = 0;
    }
    return 0;
}

int checkDiagonal(Init &player, Init &bot, Sprite &line)
{
    int playerCounter = 0, botCounter = 0;

    for (int i = 0; i < 9; i += 4)
    {
        if (player.cellMode[i])
            playerCounter++;
        else if (bot.cellMode[i])
            botCounter++;

        if (playerCounter == 3)
        {
            line.setTextureRect(IntRect(0, 10, 600, 600));
            line.setRotation(0);
            line.setPosition(0, 0);
            return PLAYER_WON;
        }
        else if (botCounter == 3)
        {
            line.setTextureRect(IntRect(0, 10, 600, 600));
            line.setRotation(0);
            line.setPosition(0, 0);
            return BOT_WON;
        }
    }

    playerCounter = 0;
    botCounter = 0;

    for (int i = 2; i < 7; i += 2)
    {
        if (player.cellMode[i])
            playerCounter++;
        else if (bot.cellMode[i])
            botCounter++;

        if (playerCounter == 3)
        {
            line.setTextureRect(IntRect(600, 10, -600, 600));
            line.setRotation(0);
            line.setPosition(0, 0);
            return PLAYER_WON;
        }
        else if (botCounter == 3)
        {
            line.setTextureRect(IntRect(600, 10, -600, 600));
            line.setRotation(0);
            line.setPosition(0, 0);
            return BOT_WON;
        }
    }
    return 0;
}

void clearCells(Init &player, Init &bot)
{
    for (int i = 0; i < 9; i++)
    {
        player.cellMode[i] = false;
        bot.cellMode[i] = false;
    }
}

bool checkCells(Init &player, Init &bot)
{
    int count = 0;
    for (int i = 0; i < 9; i++)
    {
        if (!player.cellMode[i] && !bot.cellMode[i])
            count++;
    }
    if (count > 1)
        return false;
    return true;
}

int checkResult(Init &player, Init &bot, Sprite &line)
{
    if (checkHorizontal(player, bot, line) || checkVertical(player, bot, line) || checkDiagonal(player, bot, line))
        return true;
    if (checkCells(player, bot))
        return true;
    return false;
}

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

    bool onMenu = true, onGame = false, isEnd = false;
    int playerCharacter = 0; // 0 - крестик, 1 - нолик
    int botCharacter = 0;
    bool isBotStep = false;
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
                        if (player.cells[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
                        {
                            if (!player.cellMode[i] && !bot.cellMode[i])
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

        displayButtonMode(choice, mousePosition);

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