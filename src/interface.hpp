#pragma once

#include <SFML/Graphics.hpp>

#define BOT_MOVE 100
#define PLAYER_MOVE 200
#define END_GAME 300

using namespace sf;

extern int playerCharacter, botCharacter;

typedef struct 
{
    Texture texture;
    Sprite sprite;
} Object;

typedef struct
{
    Object main[3];
    Object background;
    Object line;
} Widgets;

typedef struct
{
    Text header;
    Text start;
    Text step;
    Text result;
    Text backMenu;
} Texts;

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

void createAllWidgets(Widgets &widgets);
void createAllTextWidgets(Font &font, Texts &messages);
void changeFigureTexture(Widgets &widgets, Vector2i pos, bool &isCursorHand);
void changeButtonTexture(Widgets &widgets, Text &message, Vector2i pos, bool &isCursorHand);
void changeStepString(Text &stepMessage, int move);