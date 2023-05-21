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

    mainMsg.setString("Âûáåðè ôèãóðó");
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

    startMsg.setString("ÍÀ×ÀÒÜ ÈÃÐÓ");
    startMsg.setFont(font);
    startMsg.setCharacterSize(43);
    startMsg.setStyle(Text::Bold);
    startMsg.setPosition(135, 485);

    menuMsg.setString("ÃËÀÂÍÎÅ ÌÅÍÞ");
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

void putRandomValues()
{
    srand(time(NULL));
    playerCharacter = rand() % 2 + 1;
    (playerCharacter == 1) ? botCharacter = 2 : botCharacter = 1;
}

void makeSmartMove(int *gameField)
{
    srand(time(NULL));
    int random;
    do
    {
        random = rand() % 9;
    } while (gameField[random] || gameField[random]);
    
    gameField[random] = botCharacter;
}

int checkHorizontal(int *gameField, Sprite &line)
{
    int playerCounter = 0, botCounter = 0;
    for (int i = 0; i < 7; i += 3)
    {
        for (int j = i; j < (i + 3); j++)
        {
            if (gameField[j] == playerCharacter)
                playerCounter++;
            else if (gameField[j] == botCharacter)
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

int checkVertical(int *gameField, Sprite &line)
{
    int playerCounter = 0, botCounter = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = i; j < (i + 7); j += 3)
        {
            if (gameField[j] == playerCharacter)
                playerCounter++;
            else if (gameField[j] == botCharacter)
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

int checkDiagonal(int *gameField, Sprite &line)
{
    int playerCounter = 0, botCounter = 0;

    for (int i = 0; i < 9; i += 4)
    {
        if (gameField[i] == playerCharacter)
            playerCounter++;
        else if (gameField[i] == botCharacter)
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
        if (gameField[i] == playerCharacter)
            playerCounter++;
        else if (gameField[i] == botCharacter)
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

void clearCells(int *gameField)
{
    for (int i = 0; i < 9; i++)
    {
        gameField[i] = 0;
    }
}

int checkCells(int *gameField)
{
    int count = 0;
    for (int i = 0; i < 9; i++)
    {
        if (!gameField[i])
            count++;
    }
    if (count > 0)
        return 0;
    return DRAW;
}

int checkResult(int *gameField, Sprite &line)
{
    int result;
    if (result = checkHorizontal(gameField, line))
        return result;
    if (result = checkVertical(gameField, line))
        return result;
    if (result = checkDiagonal(gameField, line))
        return result;
    return checkCells(gameField);
}

void copyArray(int *array, int *newArray)
{
    for (int i = 0; i < 9; i++)
        newArray[i] = array[i];
}

Step findOptimalMove(int *gameField, int cross, Sprite &line)
{
    Step bestMove;
    (cross == botCharacter) ? bestMove.score = -10 : bestMove.score = 10;

    if (int result = checkResult(gameField, line))
    {
        if (result == BOT_WIN)
            bestMove.score = 1;
        else if (result == PLAYER_WIN)
            bestMove.score = -1;
        else if (result == DRAW)
            bestMove.score = 0;
    }
    else
    {
        for (int i = 0; i < 9; i++)
        {
            if (!gameField[i])
            {
                int newArray[9];
                copyArray(gameField, newArray);
                newArray[i] = cross;
                if (cross == botCharacter)
                {
                    Step currentMove = findOptimalMove(newArray, playerCharacter, line);
                    if (currentMove.score > bestMove.score)
                    {
                        bestMove.score = currentMove.score;
                        bestMove.pos = i;
                    }
                }
                else
                {
                    Step currentMove = findOptimalMove(newArray, botCharacter, line);
                    if (currentMove.score < bestMove.score)
                    {
                        bestMove.score = currentMove.score;
                        bestMove.pos = i;
                    }
                }
            }
        }
    }
    return bestMove;
}

void changeStepString(Text &stepMessage, int move)
{
    if (move == PLAYER_MOVE)
    {
        stepMessage.setFillColor(Color(84, 84, 84, 255));
        stepMessage.setPosition(235, 598);
        stepMessage.setString("Òâîé õîä!");
    }
    else if (move == BOT_MOVE)
    {
        stepMessage.setFillColor(Color(236, 234, 210, 255));
        stepMessage.setPosition(235, 598);
        stepMessage.setString("Õîä áîòà"); 
    }
    else if (move == END_GAME)
    {
        stepMessage.setFillColor(Color(84, 84, 84, 255));
        stepMessage.setPosition(185, 598);
        stepMessage.setString("Èãðà îêîí÷åíà");
    }
}