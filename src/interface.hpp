#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

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

enum class Figure : uint8_t
{
    Empty = 0,
    Cross = 1,
    Zero = 2
};

enum class Move
{
    Player,
    Bot,
    Enemy,
    Waiting,
    EndGame,
};

typedef struct
{
    Figure figure;
    Texture texture;
    Sprite sprite;
} Cell;

class Interface
{
public:
    virtual void hover(Object *button, Text &text, Vector2i mousePos);
    virtual Object **getAllObjects();
    virtual Text **getAllTexts();
    void setCursor(Cursor::Type cursorType);
    Cursor &getCursor();
    void setActive(bool active);
    bool isActive();

protected:
    virtual void createAllElements();
    Font font;
    Cursor cursor;
    Cursor::Type cursorType;
    bool active;
};

class StartWindow : public Interface
{
public:
    void hover(Object *button, Text &text, Vector2i mousePos) override;
    Object **getAllObjects() override;
    Text **getAllTexts() override;
    Text &getStartText();

    StartWindow();

protected:
    void createAllElements() override;
    Object buttons[3];
    Text headerText;
    Text startText;
};

class GameWindow : public Interface
{
public:
    Object **getAllObjects() override;
    Text **getAllTexts() override;
    void changeStepString(Move move);
    void updateCells();
    void setCell(int index, Figure figure);
    Cell *getCells();
    void setPlayerFigure(Figure playerFigure);
    Figure getPlayerFigure();
    void setEnemyFigure(Figure botFigure);
    Figure getEnemyFigure();
    void changeLinePosition(Result result);
    Sprite &getLineSprite();
    Result checkHorizontal(Cell *gameField);
    Result checkVertical(Cell *gameField);
    Result checkDiagonal(Cell *gameField);
    Result checkCells(Cell *gameField);
    Result checkResult(GameWindow &gameWindow, Cell *gameField);
    void clearCells();
    void copyArray(Cell *oldCells, Cell *newCells);

    bool getWaitMode() {
        return this->waitMode;
    }

    void setWaitMode(bool waitMode) {
        this->waitMode = waitMode;
    }

    GameWindow();

protected:
    void createAllElements() override;
    Object background;
    Object line;
    Text stepText;
    Cell cells[9];
    Figure enemyFigure;
    Figure playerFigure;
    bool botStep;
    bool randomMode;
    bool waitMode;
};

class ResultWindow : public Interface
{
public:
    void hover(Object *button, Text &text, Vector2i mousePos) override;
    Object **getAllObjects() override;
    Text **getAllTexts() override;
    void setBackMenuText(const char *string);
    Text &getBackMenuText();
    void setWaitText(const char *string);

    ResultWindow();

protected:
    void createAllElements() override;
    Object backMenuButton;
    Text winnerText;
    Text backMenuText;
    Text waitText;
};

class Player
{
public:
    void setGameWindow(GameWindow *gameWindow);
    void setFigure(Figure figure);
    Figure getFigure();
    Step findOptimalMove(Cell *gameField, bool isBotMove);

protected:
    GameWindow *gameWindow;
    Figure figure;
};