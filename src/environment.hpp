#pragma once

#include <SFML/Graphics.hpp>

#define PLAYER_WIN 1
#define BOT_WIN 2
#define DRAW 3
#define BOT_MOVE 100
#define PLAYER_MOVE 200
#define END_GAME 300

using namespace sf;

extern int playerCharacter, botCharacter;

typedef struct
{
    int score;
    int pos;
} Step;


class Init
{
public:
    Sprite cells[9]; // Всего 9 клеток

    Init(Texture &image)
    {
        for (int i = 0; i < 9; i++)
        {
            cells[i].setTexture(image);
        }
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                cells[i * 3 + j].setPosition(200 * j, 200 * i);
    }

    void update(int &figure)
    {
        for (int i = 0; i < 9; i++)
            cells[i].setTextureRect(IntRect(200 * (figure - 1), 0, 200, 200));
    }
};

void createAllWidgets(Texture &figures, Texture &buttonTexture, Sprite *choice, Texture &bkTexture, Sprite &bk, Texture &lineTexture, Sprite &line);
void createAllTextWidgets(Font &font, Text &mainMsg, Text &stepMsg, Text &winMsg, Text &startMsg, Text &menuMsg);
void changeFigureTexture(Sprite *choice, Vector2i pos, bool &isCursorHand);
void changeButtonTexture(Sprite &button, Text &message, Vector2i pos, bool &isCursorHand);
void putRandomValues();
void makeSmartMove(int *gameField);
int checkHorizontal(int *gameField, Sprite &line);
int checkVertical(int *gameField, Sprite &line);
int checkDiagonal(int *gameField, Sprite &line);
void clearCells(int *gameField);
int checkCells(int *gameField);
int checkResult(int *gameField, Sprite &line);
void copyArray(int *array, int *newArray);
Step findOptimalMove(int *gameField, int cross, Sprite &line);
void changeStepString(Text &stepMessage, int mode);