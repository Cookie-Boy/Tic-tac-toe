#pragma once

#include <SFML/Graphics.hpp>

#define PLAYER_WIN 1
#define BOT_WIN 2
#define DRAW 3

using namespace sf;

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

void hover(Sprite *choice, Vector2i pos, bool &isCursorHand);
void makeSmartMove(Init &player, Init &bot);
int checkHorizontal(Init &player, Init &bot, Sprite &line);
int checkVertical(Init &player, Init &bot, Sprite &line);
int checkDiagonal(Init &player, Init &bot, Sprite &line);
void clearCells(Init &player, Init &bot);
int checkCells(Init &player, Init &bot);
int checkResult(Init &player, Init &bot, Sprite &line);
void changeStepString(Text &stepMessage, int mode);