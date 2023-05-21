#pragma once

#include <SFML/Graphics.hpp>

#define BOT_WIN 1
#define DRAW 0
#define PLAYER_WIN -1
#define NOT_EMPTY 5

using namespace sf;

extern int playerCharacter, botCharacter;

typedef struct
{
    int score;
    int pos;
} Step;

enum class Type
{
    Horizontal,
    Vertical,
    Diagonal,
};

typedef struct
{
    Type type;
    int winner;
    int pos;
} Result;

Result checkHorizontal(int *gameField);
Result checkVertical(int *gameField);
Result checkDiagonal(int *gameField);
Result checkCells(int *gameField);
Result checkResult(int *gameField);
void clearCells(int *gameField);
void copyArray(int *array, int *newArray);
Step findOptimalMove(int *gameField, int cross);
void changeStepString(Text &stepMessage, int mode);
void changeLinePosition(Result result, Sprite &line);