#include <SFML/Graphics.hpp>

#include <interface.hpp>

// Interface

void Interface::mainButtonHover(Object &button, Text &text, Vector2i mousePos)
{
    if (button.sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
    {
        button.sprite.setTextureRect(IntRect(0, 80, 350, 80));
        text.setFillColor(Color(84, 84, 84, 255));
        isCursorHand = true;
    }
    else
    {
        button.sprite.setTextureRect(IntRect(0, 0, 350, 80));
        text.setFillColor(Color(245, 236, 211, 255));
        isCursorHand = false;
    }
}

Cursor &Interface::getCursor()
{
    return this->cursor;
}

// StartWindow

void StartWindow::figuresHover(Vector2i mousePos)
{
    for (int i = 0; i < 2; i++)
    {
        if (figureButtons[i].sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
        {
            figureButtons[i].sprite.setTextureRect(IntRect(200 * i, 200, 200, 200));
            isCursorHand = true;
            return;
        }
        else
        {
            figureButtons[i].sprite.setTextureRect(IntRect(200 * i, 0, 200, 200));
            isCursorHand = false;
        }
    }
}

Object **StartWindow::getAllObjects()
{
    Object **objects = new Object *[3];
    objects[0] = &figureButtons[0];
    objects[1] = &figureButtons[1];
    objects[2] = &startButton;
    return objects;
}

Text **StartWindow::getAllTexts()
{
    Text **texts = new Text *[2];
    texts[0] = &headerText;
    texts[1] = &startText;
    return texts;
}

Object *StartWindow::getFigureButtons()
{
    return this->figureButtons;
}


Object &StartWindow::getStartButton()
{
    return this->startButton;
}

Text &StartWindow::getStartText()
{
    return this->startText;
}

void StartWindow::createAllElements()
{
    for (int i = 0; i < 2; i++)
    {
        figureButtons[i].texture.loadFromFile("img/figures.png");
        figureButtons[i].sprite.setTexture(figureButtons[i].texture);
        figureButtons[i].sprite.setPosition(50 + 300 * i, 180);
    }

    startButton.texture.loadFromFile("img/button.png");
    startButton.sprite.setTexture(startButton.texture);
    startButton.sprite.setPosition(125, 475);

    font.loadFromFile("utils/RobotoSerif.ttf");

    headerText.setString("Âûáåðè ôèãóðó");
    headerText.setFont(font);
    headerText.setCharacterSize(60);
    headerText.setFillColor(Color::Black);
    headerText.setStyle(Text::Bold);
    headerText.setPosition(60, 50);

    startText.setString("ÍÀ×ÀÒÜ ÈÃÐÓ");
    startText.setFont(font);
    startText.setCharacterSize(43);
    startText.setStyle(Text::Bold);
    startText.setPosition(135, 485);
}

// GameField

Object **GameField::getAllObjects()
{
    Object **objects = new Object *[1];
    objects[0] = &background;
    return objects;
}

Text **GameField::getAllTexts()
{
    Text **texts = new Text *[1];
    texts[0] = &stepText;
    return texts;
}

void GameField::stepStringHover(Move move)
{
    if (move == Move::PlayerMove)
    {
        stepText.setFillColor(Color(84, 84, 84, 255));
        stepText.setPosition(227, 598);
        stepText.setString("Òâîé õîä!");
    }
    else if (move == Move::BotMove)
    {
        stepText.setFillColor(Color(236, 234, 210, 255));
        stepText.setPosition(227, 598);
        stepText.setString("Õîä áîòà"); 
    }
    else if (move == Move::EndGame)
    {
        stepText.setFillColor(Color(84, 84, 84, 255));
        stepText.setPosition(185, 598);
        stepText.setString("Èãðà îêîí÷åíà");
    }
}

void GameField::updateCells()
{
    for (int i = 0; i < 9; i++)
    {
        if (cells[i].figure == Figure::Cross || cells[i].figure == Figure::Empty)
            cells[i].cellSprite.setTextureRect(IntRect(0, 0, 200, 200));
        else if (cells[i].figure == Figure::Zero)
            cells[i].cellSprite.setTextureRect(IntRect(200, 0, 200, 200));
    }
}

void GameField::setCell(int index, Figure figure)
{
    cells[index].figure = figure;
    // cells[index].cellSprite.setTextureRect(IntRect(200 * (((int) figure) - 1), 0, 200, 200));
}

Cell *GameField::getCells()
{
    return this->cells;
}

Result GameField::checkHorizontal(Cell *gameField)
{
    Figure playerFigure = this->getPlayerFigure();
    Figure botFigure = this->getBotFigure();
    Result result;
    result.type = Type::Horizontal;
    result.winner = 0;
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

Result GameField::checkVertical(Cell *gameField)
{
    Figure playerFigure = this->getPlayerFigure();
    Figure botFigure = this->getBotFigure();
    Result result;
    result.type = Type::Vertical;
    result.winner = 0;
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

Result GameField::checkDiagonal(Cell *gameField)
{
    Figure playerFigure = this->getPlayerFigure();
    Figure botFigure = this->getBotFigure();
    Result result;
    result.type = Type::Diagonal;
    result.winner = 0;
    int playerCounter = 0, botCounter = 0;

    for (int i = 0; i < 9; i += 4)
    {
        if (gameField[i].figure == playerFigure)
            playerCounter++;
        else if (gameField[i].figure == botFigure)
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
        if (gameField[i].figure == playerFigure)
            playerCounter++;
        else if (gameField[i].figure == botFigure)
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

Result GameField::checkCells(Cell *gameField)
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
        result.winner = NOT_EMPTY;
        return result;
    }
    result.winner = DRAW;
    return result;
}

Result GameField::checkResult(GameField &gameObject, Cell *gameField)
{
    Result result;
    if ((result = gameObject.checkHorizontal(gameField)).winner)
        return result;
    if ((result = gameObject.checkVertical(gameField)).winner)
        return result;
    if ((result = gameObject.checkDiagonal(gameField)).winner)
        return result;
    return gameObject.checkCells(gameField);
}

void GameField::clearCells(GameField &gameField)
{
    for (int i = 0; i < 9; i++)
        gameField.getCells()[i].figure = Figure::Empty;
}

void copyArray(Cell *oldCells, Cell *newCells)
{
    for (int i = 0; i < 9; i++)
        newCells[i].figure = oldCells[i].figure;
}

void GameField::setPlayerFigure(Figure playerFigure)
{
    this->playerFigure = playerFigure;
}

Figure GameField::getPlayerFigure()
{
    return this->playerFigure;
}

void GameField::setBotFigure(Figure botFigure)
{
    this->botFigure = botFigure;
}

Figure GameField::getBotFigure()
{
    return this->botFigure;
}

void GameField::changeLinePosition(Result result)
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

Sprite &GameField::getLineSprite()
{
    return this->line.sprite;
}

void GameField::createAllElements()
{
    // Texture image;
    // image.loadFromFile("img/figures.png");

    for (int i = 0; i < 9; i++)
    {
        cells[i].cellTexture.loadFromFile("img/figures.png");
        cells[i].cellSprite.setTexture(cells[i].cellTexture);
        cells[i].figure = Figure::Empty;

    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cells[i * 3 + j].cellSprite.setPosition(200 * j, 200 * i);
        }
    }

    font.loadFromFile("utils/RobotoSerif.ttf");
    stepText.setFont(font);
    stepText.setCharacterSize(30);
    stepText.setStyle(Text::Bold);

    background.texture.loadFromFile("img/background.png");
    background.sprite.setTexture(background.texture);
    line.texture.loadFromFile("img/line.png");
    line.sprite.setTexture(line.texture);
}

Object **ResultWindow::getAllObjects()
{
    Object **objects = new Object *[1];
    objects[0] = &backMenuButton;
    return objects;
}

Text **ResultWindow::getAllTexts()
{
    Text **texts = new Text *[2];
    texts[0] = &winnerText;
    texts[1] = &backMenuText;
    return texts;
}

Object ResultWindow::getBackMenuButton()
{
    return this->backMenuButton;
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

    backMenuText.setString("ÃËÀÂÍÎÅ ÌÅÍÞ");
    backMenuText.setFont(font);
    backMenuText.setCharacterSize(37);
    backMenuText.setStyle(Text::Bold);
    backMenuText.setPosition(130, 490);
}

void Player::setFigure(Figure figure)
{
    this->figure = figure;
}

Figure Player::getFigure()
{
    return this->figure;
}

Step findOptimalMove(GameField &gameObject, Cell *gameField, bool isBot)
{
    Step bestMove;
    (isBot) ? bestMove.score = -10 : bestMove.score = 10;
    Result result = gameObject.checkResult(gameObject, gameField);
    // Result result = gameField.checkResult(gameField.getPlayerFigure(), gameField.getBotFigure());

    if (result.winner != NOT_EMPTY)
    {
        bestMove.score = result.winner;
    }
    else
    {
        for (int i = 0; i < 9; i++)
        {
            if (gameField[i].figure == Figure::Empty)
            {
                Cell newField[9];
                copyArray(gameField, newField);
                newField[i].figure = isBot ? gameObject.getBotFigure() : gameObject.getPlayerFigure();

                // for (int i = 0; i < 9; i++)
                // {
                //     if (newField.getCells()[i].figure == Figure::Cross)
                //         cout << "X ";
                //     else if (newField.getCells()[i].figure == Figure::Zero)
                //         cout << "O ";
                //     else if (newField.getCells()[i].figure == Figure::Empty)
                //         cout << "E ";
                // }
                // cout << endl << "i = " << i << endl;

                if (isBot)
                {
                    Step currentMove = findOptimalMove(gameObject, newField, false);
                    if (currentMove.score > bestMove.score)
                    {
                        bestMove.score = currentMove.score;
                        bestMove.pos = i;
                    }
                }
                else
                {
                    Step currentMove = findOptimalMove(gameObject, newField, true);
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