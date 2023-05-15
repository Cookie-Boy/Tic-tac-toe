#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

#include <environment.hpp>

using namespace std;

void createAllWidgets(Texture &figures, Texture &buttonTexture, Sprite *choice, Texture &bkTexture, Sprite &bk, Texture &lineTexture, Sprite &line)
{
    figures.loadFromFile("img/figures.png");
    for (int i = 0; i < 2; i++)
    {
        choice[i].setTexture(figures);
        choice[i].setPosition(50 + 300 * i, 180);
    }

    buttonTexture.loadFromFile("img/button.png");
    choice[2].setTexture(buttonTexture);
    choice[2].setPosition(125, 475);

    bkTexture.loadFromFile("img/background.png");
    bk.setTexture(bkTexture);

    lineTexture.loadFromFile("img/line.png");
    line.setTexture(lineTexture);
}

void createAllTextWidgets(Font &font, Text &mainMsg, Text &stepMsg, Text &winMsg, Text &startMsg, Text &menuMsg)
{
    font.loadFromFile("utils/RobotoSerif.ttf");

    mainMsg.setString("Выбери фигуру");
    mainMsg.setFont(font);
    mainMsg.setCharacterSize(60);
    mainMsg.setFillColor(Color::Black);
    mainMsg.setStyle(Text::Bold);
    mainMsg.setPosition(60, 50);

    stepMsg.setFont(font);
    stepMsg.setCharacterSize(30);
    stepMsg.setStyle(Text::Bold);

    winMsg.setFont(font);
    winMsg.setCharacterSize(75);
    winMsg.setFillColor(Color::Black);
    winMsg.setStyle(Text::Bold);

    startMsg.setString("НАЧАТЬ ИГРУ");
    startMsg.setFont(font);
    startMsg.setCharacterSize(43);
    startMsg.setStyle(Text::Bold);
    startMsg.setPosition(135, 485);

    menuMsg.setString("ГЛАВНОЕ МЕНЮ");
    menuMsg.setFont(font);
    menuMsg.setCharacterSize(37);
    menuMsg.setStyle(Text::Bold);
    menuMsg.setPosition(130, 490);
}

void changeFigureTexture(Sprite *choice, Vector2i pos, bool &isCursorHand)
{
    for (int i = 0; i < 2; i++)
    {
        if (choice[i].getGlobalBounds().contains(pos.x, pos.y))
        {
            choice[i].setTextureRect(IntRect(200 * i, 200, 200, 200));
            isCursorHand = true;
            return;
        }
        else
        {
            choice[i].setTextureRect(IntRect(200 * i, 0, 200, 200));
            isCursorHand = false;
        }
    }
}

void changeButtonTexture(Sprite &button, Text &message, Vector2i pos, bool &isCursorHand)
{
    if (button.getGlobalBounds().contains(pos.x, pos.y))
    {
        button.setTextureRect(IntRect(0, 80, 350, 80));
        message.setFillColor(Color(84, 84, 84, 255));
        isCursorHand = true;
    }
    else
    {
        button.setTextureRect(IntRect(0, 0, 350, 80));
        message.setFillColor(Color(245, 236, 211, 255));
        isCursorHand = false;
    }
}

void putRandomValues(int &playerCharacter, int &botCharacter)
{
    srand(time(NULL));
    playerCharacter = rand() % 1;
    botCharacter = !playerCharacter;
}

void makeSmartMove(Init &player, Init &bot)
{
    srand(time(NULL));
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
                return PLAYER_WIN;
            }
            else if (botCounter == 3)
            {
                line.setTextureRect(IntRect(0, 0, 600, 10));
                line.setRotation(0);
                int distance = ((i / 3) * 200) + 100;
                line.setPosition(0, distance);
                return BOT_WIN;
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
                return PLAYER_WIN;
            }
            else if (botCounter == 3)
            {
                line.setTextureRect(IntRect(0, 0, 600, 10));
                line.setRotation(90);
                int distance = (i * 200) + 105;
                line.setPosition(distance, 0);
                return BOT_WIN;
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
            return PLAYER_WIN;
        }
        else if (botCounter == 3)
        {
            line.setTextureRect(IntRect(0, 10, 600, 600));
            line.setRotation(0);
            line.setPosition(0, 0);
            return BOT_WIN;
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
            return PLAYER_WIN;
        }
        else if (botCounter == 3)
        {
            line.setTextureRect(IntRect(600, 10, -600, 600));
            line.setRotation(0);
            line.setPosition(0, 0);
            return BOT_WIN;
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

int checkCells(Init &player, Init &bot)
{
    int count = 0;
    for (int i = 0; i < 9; i++)
    {
        if (!player.cellMode[i] && !bot.cellMode[i])
            count++;
    }
    if (count > 0)
        return 0;
    return DRAW;
}

int checkResult(Init &player, Init &bot, Sprite &line)
{
    int result;
    if (result = checkHorizontal(player, bot, line))
        return result;
    if (result = checkVertical(player, bot, line))
        return result;
    if (result = checkDiagonal(player, bot, line))
        return result;
    return checkCells(player, bot);
}

void changeStepString(Text &stepMessage, int move)
{
    if (move == PLAYER_MOVE)
    {
        stepMessage.setFillColor(Color(84, 84, 84, 255));
        stepMessage.setPosition(235, 598);
        stepMessage.setString("Твой ход!");
    }
    else if (move == BOT_MOVE)
    {
        stepMessage.setFillColor(Color(236, 234, 210, 255));
        stepMessage.setPosition(235, 598);
        stepMessage.setString("Ход бота"); 
    }
    else if (move == END_GAME)
    {
        stepMessage.setFillColor(Color(84, 84, 84, 255));
        stepMessage.setPosition(185, 598);
        stepMessage.setString("Игра окончена");
    }
}