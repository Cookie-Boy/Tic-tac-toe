#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#define BOT_WIN 1
#define DRAW 0
#define PLAYER_WIN -1
#define NOT_EMPTY 5

using namespace sf;
using namespace std;

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

typedef struct 
{
    Texture texture;
    Sprite sprite;
} Object;

enum class Figure
{
    Empty,
    Cross,
    Zero,
};

enum class Move
{
    PlayerMove,
    BotMove,
    EndGame,
};

typedef struct
{
    Figure figure;
    Texture cellTexture;
    Sprite cellSprite;
} Cell;

class Interface
{
public:
    void mainButtonHover(Object &button, Text &text, Vector2i mousePos);
    virtual Object **getAllObjects()
    {

    }
    virtual Text **getAllTexts()
    {

    }
    Cursor &getCursor();
    
protected:
    virtual void createAllElements()
    {
    } // � ������� ����
    Font font;
    Cursor cursor;
    bool isCursorHand;
};

class StartWindow : public Interface
{
public:
    void figuresHover(Vector2i mousePos);
    Object **getAllObjects() override;
    Text **getAllTexts() override;
    Object *getFigureButtons();
    Object &getStartButton();
    Text &getStartText();
    StartWindow()
    {
        createAllElements();
    }
protected:
    void createAllElements() override;
    Object figureButtons[2];
    Object startButton;
    Text headerText;
    Text startText;
};

class GameField : public Interface
{
public:
    Object **getAllObjects() override;
    Text **getAllTexts() override;
    void stepStringHover(Move move);
    void updateCells();
    void setCell(int index, Figure figure);
    Cell *getCells();
    void setPlayerFigure(Figure playerFigure);
    Figure getPlayerFigure();
    void setBotFigure(Figure botFigure);
    Figure getBotFigure();
    void changeLinePosition(Result result);
    Sprite &getLineSprite();
    Result checkHorizontal(Cell *gameField);
    Result checkVertical(Cell *gameField);
    Result checkDiagonal(Cell *gameField);
    Result checkCells(Cell *gameField);
    Result checkResult(GameField &gameObject, Cell *gameField);
    void clearCells(GameField &gameField);
    GameField()
    {
        createAllElements();
    }

protected:

    void createAllElements() override;
    Object background;
    Object line;
    Text stepText;
    Cell cells[9];
    Figure botFigure;
    Figure playerFigure;
};

class ResultWindow : public Interface
{
public:
    Object **getAllObjects() override;
    Text **getAllTexts() override;
    Object getBackMenuButton();
    ResultWindow()
    {
        createAllElements();
    }
protected:
    void createAllElements() override;
    Object backMenuButton;
    Text winnerText;
    Text backMenuText;
};

class Player
{
public:
    void setFigure(Figure figure);
    Figure getFigure();
    // Step findOptimalMove(GameField &gameField, Figure figure);

    Player(bool isBot = false)
    {
        this->isBot = isBot;
    }

protected:
    Figure figure;
    bool isBot;
};

void copyArray(Cell *oldCells, Cell *newCells);
Step findOptimalMove(GameField &gameObject, Cell *gameField, bool isBot);