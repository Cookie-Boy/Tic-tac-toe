#include <SFML/Graphics.hpp>
#include <cstring>

#include <interface.hpp>

// Interface

void Interface::hover(Object *button, Text &text, Vector2i mousePos)
{
}

Object **Interface::getAllObjects()
{
}

Text **Interface::getAllTexts()
{
}

void Interface::setCursor(Cursor::Type cursorType)
{
    if (cursorType != this->cursorType)
    {
        this->cursor.loadFromSystem(cursorType);
        this->cursorType = cursorType;
    }
}

Cursor &Interface::getCursor()
{
    return this->cursor;
}

void Interface::setActive(bool active)
{
    this->active = active;
}

bool Interface::isActive()
{
    return this->active;
}

void Interface::createAllElements()
{
}

// StartWindow

void StartWindow::hover(Object *button, Text &text, Vector2i mousePos)
{
    for (int i = 0; i < 2; i++)
    {
        if (buttons[i].sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
        {
            buttons[i].sprite.setTextureRect(IntRect(200 * i, 200, 200, 200));
            setCursor(Cursor::Hand);
            return;
        }
        else
        {
            buttons[i].sprite.setTextureRect(IntRect(200 * i, 0, 200, 200));
        }
    }

    if (button->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
    {
        button->sprite.setTextureRect(IntRect(0, 80, 350, 80));
        text.setFillColor(Color(84, 84, 84, 255));
        setCursor(Cursor::Hand);
        return;
    }
    else
    {
        button->sprite.setTextureRect(IntRect(0, 0, 350, 80));
        text.setFillColor(Color(245, 236, 211, 255));
    }

    setCursor(Cursor::Arrow);
}

Object **StartWindow::getAllObjects()
{
    static Object *objects[3];
    for (int i = 0; i < 3; i++)
        objects[i] = &buttons[i];
    return objects;
}

Text **StartWindow::getAllTexts()
{
    static Text *texts[2];
    texts[0] = &headerText;
    texts[1] = &startText;
    return texts;
}

Text &StartWindow::getStartText()
{
    return this->startText;
}

// Constructor for StartWindow

StartWindow::StartWindow()
{
    createAllElements();
    this->cursorType = Cursor::Arrow;
    this->cursor.loadFromSystem(cursorType);
    this->active = true;
}

void StartWindow::createAllElements()
{
    for (int i = 0; i < 3; i++)
    {
        if (i == 2)
        {
            buttons[i].texture.loadFromFile("img/button.png");
            buttons[i].sprite.setTexture(buttons[i].texture);
            buttons[i].sprite.setPosition(125, 475);
        }
        else
        {
            buttons[i].texture.loadFromFile("img/figures.png");
            buttons[i].sprite.setTexture(buttons[i].texture);
            buttons[i].sprite.setPosition(50 + 300 * i, 180);
        }
    }

    font.loadFromFile("utils/RobotoSerif.ttf");

    headerText.setString("SELECT FIGURE");
    headerText.setFont(font);
    headerText.setCharacterSize(60);
    headerText.setFillColor(Color::Black);
    headerText.setStyle(Text::Bold);
    headerText.setPosition(60, 50);

    startText.setString("START GAME");
    startText.setFont(font);
    startText.setCharacterSize(43);
    startText.setStyle(Text::Bold);
    startText.setPosition(135, 485);
}

// GameWindow

Object **GameWindow::getAllObjects()
{
    static Object *objects[1];
    objects[0] = &background;
    return objects;
}

Text **GameWindow::getAllTexts()
{
    static Text *texts[1];
    texts[0] = &stepText;
    return texts;
}

void GameWindow::changeStepString(Move move)
{
    if (move == Move::Player)
    {
        stepText.setFillColor(Color(84, 84, 84, 255));
        stepText.setPosition(227, 598);
        stepText.setString("Your step!");
    }
    else if (move == Move::Bot)
    {
        stepText.setFillColor(Color(236, 234, 210, 255));
        stepText.setPosition(227, 598);
        stepText.setString("Bot's step!");
    }
    else if (move == Move::Enemy)
    {
        stepText.setFillColor(Color(236, 234, 210, 255));
        stepText.setPosition(227, 598);
        stepText.setString("Player 2 step!");
    }
    else if (move == Move::Waiting)
    {
        stepText.setFillColor(Color(236, 234, 210, 255));
        stepText.setPosition(227, 598);
        stepText.setString("Waiting for player...");
    }
    else if (move == Move::EndGame)
    {
        stepText.setFillColor(Color(84, 84, 84, 255));
        stepText.setPosition(185, 598);
        stepText.setString("End game!");
    }
}

void GameWindow::updateCells()
{
    for (int i = 0; i < 9; i++)
    {
        if (cells[i].figure == Figure::Cross || cells[i].figure == Figure::Empty)
            cells[i].sprite.setTextureRect(IntRect(0, 0, 200, 200));
        else if (cells[i].figure == Figure::Zero)
            cells[i].sprite.setTextureRect(IntRect(200, 0, 200, 200));
    }
}

void GameWindow::setCell(int index, Figure figure)
{
    cells[index].figure = figure;
}

Cell *GameWindow::getCells()
{
    return this->cells;
}

void GameWindow::setPlayerFigure(Figure playerFigure)
{
    this->playerFigure = playerFigure;
}

Figure GameWindow::getPlayerFigure()
{
    return this->playerFigure;
}

void GameWindow::setEnemyFigure(Figure enemyFigure)
{
    this->enemyFigure = enemyFigure;
}

Figure GameWindow::getEnemyFigure()
{
    return this->enemyFigure;
}

void GameWindow::changeLinePosition(Result result)
{
    if (result.type == Type::Horizontal)
    {
        line.sprite.setTextureRect(IntRect(0, 0, 600, 10));
        line.sprite.setRotation(0);
        int distance = ((result.pos / 3) * 200) + 100;
        line.sprite.setPosition(0, distance);
    }
    else if (result.type == Type::Vertical)
    {
        line.sprite.setTextureRect(IntRect(0, 0, 600, 10));
        line.sprite.setRotation(90);
        int distance = (result.pos * 200) + 105;
        line.sprite.setPosition(distance, 0);
    }
    else if (result.type == Type::Diagonal)
    {
        if (result.pos == 1)
            line.sprite.setTextureRect(IntRect(0, 10, 600, 600));
        else if (result.pos == 2)
            line.sprite.setTextureRect(IntRect(600, 10, -600, 600));
        line.sprite.setRotation(0);
        line.sprite.setPosition(0, 0);
    }
}

Sprite &GameWindow::getLineSprite()
{
    return this->line.sprite;
}

Result GameWindow::checkHorizontal(Cell *gameField)
{
    Figure playerFigure = this->getPlayerFigure();
    Figure botFigure = this->getEnemyFigure();
    Result result;
    result.type = Type::Horizontal;
    result.winner = Winner::Unknown;
    int playerCounter = 0, botCounter = 0;

    for (int i = 0; i < 7; i += 3)
    {
        for (int j = i; j < (i + 3); j++)
        {
            if (gameField[j].figure == playerFigure)
                playerCounter++;
            else if (gameField[j].figure == botFigure)
                botCounter++;

            if (playerCounter == 3)
            {
                result.winner = Winner::Player;
                result.pos = i;
                return result;
            }
            else if (botCounter == 3)
            {
                result.winner = Winner::Bot;
                result.pos = i;
                return result;
            }
        }
        playerCounter = 0;
        botCounter = 0;
    }
    return result;
}

Result GameWindow::checkVertical(Cell *gameField)
{
    Figure playerFigure = this->getPlayerFigure();
    Figure botFigure = this->getEnemyFigure();
    Result result;
    result.type = Type::Vertical;
    result.winner = Winner::Unknown;
    int playerCounter = 0, botCounter = 0;

    for (int i = 0; i < 3; i++)
    {
        for (int j = i; j < (i + 7); j += 3)
        {
            if (gameField[j].figure == playerFigure)
                playerCounter++;
            else if (gameField[j].figure == botFigure)
                botCounter++;

            if (playerCounter == 3)
            {
                result.winner = Winner::Player;
                result.pos = i;
                return result;
            }
            else if (botCounter == 3)
            {
                result.winner = Winner::Bot;
                result.pos = i;
                return result;
            }
        }
        playerCounter = 0;
        botCounter = 0;
    }
    return result;
}

Result GameWindow::checkDiagonal(Cell *gameField)
{
    Figure playerFigure = this->getPlayerFigure();
    Figure botFigure = this->getEnemyFigure();
    Result result;
    result.type = Type::Diagonal;
    result.winner = Winner::Unknown;
    int playerCounter = 0, botCounter = 0;

    for (int i = 0; i < 9; i += 4)
    {
        if (gameField[i].figure == playerFigure)
            playerCounter++;
        else if (gameField[i].figure == botFigure)
            botCounter++;

        if (playerCounter == 3)
        {
            result.winner = Winner::Player;
            result.pos = 1;
            return result;
        }
        else if (botCounter == 3)
        {
            result.winner = Winner::Bot;
            result.pos = 1;
            return result;
        }
    }

    playerCounter = 0;
    botCounter = 0;

    for (int i = 2; i < 7; i += 2)
    {
        if (gameField[i].figure == playerFigure)
            playerCounter++;
        else if (gameField[i].figure == botFigure)
            botCounter++;

        if (playerCounter == 3)
        {
            result.winner = Winner::Player;
            result.pos = 2;
            return result;
        }
        else if (botCounter == 3)
        {
            result.winner = Winner::Bot;
            result.pos = 2;
            return result;
        }
    }
    return result;
}

Result GameWindow::checkCells(Cell *gameField)
{
    Result result;
    int count = 0;
    for (int i = 0; i < 9; i++)
    {
        if (gameField[i].figure == Figure::Empty)
            count++;
    }
    if (count > 0)
    {
        result.winner = Winner::Unknown;
        return result;
    }
    result.winner = Winner::Draw;
    return result;
}

Result GameWindow::checkResult(GameWindow &gameWindow, Cell *gameField)
{
    Result result;
    if ((result = gameWindow.checkHorizontal(gameField)).winner != Winner::Unknown)
        return result;
    if ((result = gameWindow.checkVertical(gameField)).winner != Winner::Unknown)
        return result;
    if ((result = gameWindow.checkDiagonal(gameField)).winner != Winner::Unknown)
        return result;
    return checkCells(gameField);
}

void GameWindow::clearCells()
{
    for (int i = 0; i < 9; i++)
        this->cells[i].figure = Figure::Empty;
}

void GameWindow::copyArray(Cell *oldCells, Cell *newCells)
{
    for (int i = 0; i < 9; i++)
        newCells[i].figure = oldCells[i].figure;
}

// Constructor for GameWindow

GameWindow::GameWindow()
{
    createAllElements();
    this->cursorType = Cursor::Arrow;
    this->cursor.loadFromSystem(cursorType);
    this->active = false;
}

void GameWindow::createAllElements()
{
    for (int i = 0; i < 9; i++)
    {
        cells[i].texture.loadFromFile("img/figures.png");
        cells[i].sprite.setTexture(cells[i].texture);
        cells[i].figure = Figure::Empty;
    }

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            cells[i * 3 + j].sprite.setPosition(200 * j, 200 * i);

    font.loadFromFile("utils/RobotoSerif.ttf");
    stepText.setFont(font);
    stepText.setCharacterSize(30);
    stepText.setStyle(Text::Bold);

    background.texture.loadFromFile("img/background.png");
    background.sprite.setTexture(background.texture);
    line.texture.loadFromFile("img/line.png");
    line.sprite.setTexture(line.texture);
}

void ResultWindow::hover(Object *button, Text &text, Vector2i mousePos)
{
    if (button->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
    {
        button->sprite.setTextureRect(IntRect(0, 80, 350, 80));
        text.setFillColor(Color(84, 84, 84, 255));
        setCursor(Cursor::Hand);
        return;
    }
    else
    {
        button->sprite.setTextureRect(IntRect(0, 0, 350, 80));
        text.setFillColor(Color(245, 236, 211, 255));
    }

    setCursor(Cursor::Arrow);
}

Object **ResultWindow::getAllObjects()
{
    static Object *objects[1];
    objects[0] = &backMenuButton;
    return objects;
}

Text **ResultWindow::getAllTexts()
{
    static Text *texts[2];
    texts[0] = &winnerText;
    texts[1] = &backMenuText;
    return texts;
}

void ResultWindow::setBackMenuText(const char *string)
{
    this->winnerText.setString(string);
    if (!strcmp("WINNER!", string))
        this->winnerText.setPosition(15, 300);
    else
        this->winnerText.setPosition(150, 300);
}

Text &ResultWindow::getBackMenuText()
{
    return this->backMenuText;
}

// Constructor for ResultWindow

ResultWindow::ResultWindow()
{
    createAllElements();
    this->cursorType = Cursor::Arrow;
    this->cursor.loadFromSystem(cursorType);
    this->active = false;
}

void ResultWindow::createAllElements()
{
    backMenuButton.texture.loadFromFile("img/button.png");
    backMenuButton.sprite.setTexture(backMenuButton.texture);
    backMenuButton.sprite.setPosition(125, 475);

    font.loadFromFile("utils/RobotoSerif.ttf");

    winnerText.setFont(font);
    winnerText.setCharacterSize(75);
    winnerText.setFillColor(Color::Black);
    winnerText.setStyle(Text::Bold);

    backMenuText.setString("MAIN MENU");
    backMenuText.setFont(font);
    backMenuText.setCharacterSize(37);
    backMenuText.setStyle(Text::Bold);
    backMenuText.setPosition(130, 490);
}

void Player::setGameWindow(GameWindow *gameWindow)
{
    this->gameWindow = gameWindow;
}

void Player::setFigure(Figure figure)
{
    this->figure = figure;
}

Figure Player::getFigure()
{
    return this->figure;
}

Step Player::findOptimalMove(Cell *gameField, bool isBotMove)
{
    Step bestMove;
    isBotMove ? bestMove.score = -10 : bestMove.score = 10;
    Result result = gameWindow->checkResult(*gameWindow, gameField);

    if (result.winner != Winner::Unknown)
    {
        bestMove.score = (int)result.winner;
        return bestMove;
    }
    
    for (int i = 0; i < 9; i++)
    {
        if (gameField[i].figure == Figure::Empty)
        {
            Cell newField[9];
            gameWindow->copyArray(gameField, newField);
            newField[i].figure = isBotMove ? gameWindow->getEnemyFigure() : gameWindow->getPlayerFigure();

            if (isBotMove)
            {
                Step currentMove = findOptimalMove(newField, false);
                if (currentMove.score > bestMove.score)
                {
                    bestMove.score = currentMove.score;
                    bestMove.pos = i;
                }
            }
            else
            {
                Step currentMove = findOptimalMove(newField, true);
                if (currentMove.score < bestMove.score)
                {
                    bestMove.score = currentMove.score;
                    bestMove.pos = i;
                }
            }
        }
    }
    return bestMove;
}