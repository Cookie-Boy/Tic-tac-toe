#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

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

enum class Winner
{
    Player = -1,
    Draw,
    Bot,
    Unknown,
};

typedef struct
{
    Type type;
    Winner winner;
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
    virtual void hover(Object *button, Text &text, Vector2i mousePos)
    {

    }
    virtual Object **getAllObjects()
    {

    }
    virtual Text **getAllTexts()
    {

    }
    Cursor &getCursor();
    void setCursor(Cursor::Type cursorType);
    
protected:
    virtual void createAllElements()
    {

    }
    Font font;
    Cursor cursor;
    bool isCursorHand;
    Cursor::Type cursorType;
};

class StartWindow : public Interface
{
public:
    void hover(Object *button, Text &text, Vector2i mousePos) override;
    void figuresHover(Vector2i mousePos);
    Object **getAllObjects() override;
    Text **getAllTexts() override;
    Text &getStartText();
    StartWindow()
    {
        createAllElements();
        this->cursorType = Cursor::Arrow;
        this->cursor.loadFromSystem(cursorType);
    }
protected:
    void createAllElements() override;
    Object buttons[3];
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
    void copyArray(Cell *oldCells, Cell *newCells);
    GameField()
    {
        createAllElements();
        this->cursorType = Cursor::Arrow;
        this->cursor.loadFromSystem(cursorType);
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
    void hover(Object *button, Text &text, Vector2i mousePos) override;
    Object **getAllObjects() override;
    Text **getAllTexts() override;
    Object getBackMenuButton();
    void setBackMenuText(const char *string);
    Text &getBackMenuText();
    ResultWindow()
    {
        createAllElements();
        this->cursorType = Cursor::Arrow;
        this->cursor.loadFromSystem(cursorType);
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
    Step findOptimalMove(GameField &gameObject, Cell *gameField, bool isBot);

    Player(bool isBot = false)
    {
        this->isBot = isBot;
    }

protected:
    Figure figure;
    bool isBot;
};