#include <SFML/Graphics.hpp>
#include <time.h>

#include <environment.hpp>

void hover(Sprite *choice, Vector2i pos)
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