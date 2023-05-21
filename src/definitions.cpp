#include <SFML/Graphics.hpp>

#include <definitions.hpp>

Result checkHorizontal(int *gameField)
{
    Result result;
    result.type = Type::Horizontal;
    result.winner = 0;
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
                result.winner = PLAYER_WIN;
                result.pos = i;
                return result;
            }
            else if (botCounter == 3)
            {
                result.winner = BOT_WIN;
                result.pos = i;
                return result;
            }
        }
        playerCounter = 0;
        botCounter = 0;
    }
    return result;
}

Result checkVertical(int *gameField)
{
    Result result;
    result.type = Type::Vertical;
    result.winner = 0;
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
                result.winner = PLAYER_WIN;
                result.pos = i;
                return result;
            }
            else if (botCounter == 3)
            {
                result.winner = BOT_WIN;
                result.pos = i;
                return result;
            }
        }
        playerCounter = 0;
        botCounter = 0;
    }
    return result;
}

Result checkDiagonal(int *gameField)
{
    Result result;
    result.type = Type::Diagonal;
    result.winner = 0;
    int playerCounter = 0, botCounter = 0;

    for (int i = 0; i < 9; i += 4)
    {
        if (gameField[i] == playerCharacter)
            playerCounter++;
        else if (gameField[i] == botCharacter)
            botCounter++;

        if (playerCounter == 3)
        {
            result.winner = PLAYER_WIN;
            result.pos = 1;
            return result;
        }
        else if (botCounter == 3)
        {
            result.winner = BOT_WIN;
            result.pos = 1;
            return result;
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
            result.winner = PLAYER_WIN;
            result.pos = 2;
            return result;
        }
        else if (botCounter == 3)
        {
            result.winner = BOT_WIN;
            result.pos = 2;
            return result;
        }
    }
    return result;
}

Result checkCells(int *gameField)
{
    Result result;
    int count = 0;
    for (int i = 0; i < 9; i++)
    {
        if (!gameField[i])
            count++;
    }
    if (count > 0)
    {
        result.winner = NOT_EMPTY;
        return result;
    }
    result.winner = DRAW;
    return result;
}

Result checkResult(int *gameField)
{
    Result result;
    if ((result = checkHorizontal(gameField)).winner)
        return result;
    if ((result = checkVertical(gameField)).winner)
        return result;
    if ((result = checkDiagonal(gameField)).winner)
        return result;
    return checkCells(gameField);
}

void clearCells(int *gameField)
{
    for (int i = 0; i < 9; i++)
        gameField[i] = 0;
}

void copyArray(int *array, int *newArray)
{
    for (int i = 0; i < 9; i++)
        newArray[i] = array[i];
}

Step findOptimalMove(int *gameField, int cross)
{
    Step bestMove;
    (cross == botCharacter) ? bestMove.score = -10 : bestMove.score = 10;
    Result result = checkResult(gameField);

    if (result.winner != NOT_EMPTY)
    {
        bestMove.score = result.winner;
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
                    Step currentMove = findOptimalMove(newArray, playerCharacter);
                    if (currentMove.score > bestMove.score)
                    {
                        bestMove.score = currentMove.score;
                        bestMove.pos = i;
                    }
                }
                else
                {
                    Step currentMove = findOptimalMove(newArray, botCharacter);
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

void changeLinePosition(Result result, Sprite &line)
{
    if (result.type == Type::Horizontal)
    {
        line.setTextureRect(IntRect(0, 0, 600, 10));
        line.setRotation(0);
        int distance = ((result.pos / 3) * 200) + 100;
        line.setPosition(0, distance);
    }
    else if (result.type == Type::Vertical)
    {
        line.setTextureRect(IntRect(0, 0, 600, 10));
        line.setRotation(90);
        int distance = (result.pos * 200) + 105;
        line.setPosition(distance, 0);
    }
    else if (result.type == Type::Diagonal)
    {
        if (result.pos == 1)
            line.setTextureRect(IntRect(0, 10, 600, 600));
        else if (result.pos == 2)
            line.setTextureRect(IntRect(600, 10, -600, 600));
        line.setRotation(0);
        line.setPosition(0, 0);
    }
}