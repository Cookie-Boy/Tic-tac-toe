#include <SFML/Graphics.hpp>

#include <interface.hpp>

void createAllWidgets(Widgets &widgets)
{
    for (int i = 0; i < 3; i++)
    {
        if (i == 2)
        {
            widgets.main[i].texture.loadFromFile("img/button.png");
            widgets.main[i].sprite.setTexture(widgets.main[i].texture);
            widgets.main[i].sprite.setPosition(125, 475);
        }
        else
        {
            widgets.main[i].texture.loadFromFile("img/figures.png");
            widgets.main[i].sprite.setTexture(widgets.main[i].texture);
            widgets.main[i].sprite.setPosition(50 + 300 * i, 180);
        }
    }

    widgets.background.texture.loadFromFile("img/background.png");
    widgets.background.sprite.setTexture(widgets.background.texture);
    widgets.line.texture.loadFromFile("img/line.png");
    widgets.line.sprite.setTexture(widgets.line.texture);
}

void createAllTextWidgets(Font &font, Texts &messages)
{
    font.loadFromFile("utils/RobotoSerif.ttf");

    messages.header.setString("Âûáåðè ôèãóðó");
    messages.header.setFont(font);
    messages.header.setCharacterSize(60);
    messages.header.setFillColor(Color::Black);
    messages.header.setStyle(Text::Bold);
    messages.header.setPosition(60, 50);

    messages.start.setString("ÍÀ×ÀÒÜ ÈÃÐÓ");
    messages.start.setFont(font);
    messages.start.setCharacterSize(43);
    messages.start.setStyle(Text::Bold);
    messages.start.setPosition(135, 485);

    messages.step.setFont(font);
    messages.step.setCharacterSize(30);
    messages.step.setStyle(Text::Bold);

    messages.result.setFont(font);
    messages.result.setCharacterSize(75);
    messages.result.setFillColor(Color::Black);
    messages.result.setStyle(Text::Bold);

    messages.backMenu.setString("ÃËÀÂÍÎÅ ÌÅÍÞ");
    messages.backMenu.setFont(font);
    messages.backMenu.setCharacterSize(37);
    messages.backMenu.setStyle(Text::Bold);
    messages.backMenu.setPosition(130, 490);
}

void changeFigureTexture(Widgets &widgets, Vector2i pos, bool &isCursorHand)
{
    for (int i = 0; i < 2; i++)
    {
        if (widgets.main[i].sprite.getGlobalBounds().contains(pos.x, pos.y))
        {
            widgets.main[i].sprite.setTextureRect(IntRect(200 * i, 200, 200, 200));
            isCursorHand = true;
            return;
        }
        else
        {
            widgets.main[i].sprite.setTextureRect(IntRect(200 * i, 0, 200, 200));
            isCursorHand = false;
        }
    }
}

void changeButtonTexture(Widgets &widgets, Text &message, Vector2i pos, bool &isCursorHand)
{
    if (widgets.main[2].sprite.getGlobalBounds().contains(pos.x, pos.y))
    {
        widgets.main[2].sprite.setTextureRect(IntRect(0, 80, 350, 80));
        message.setFillColor(Color(84, 84, 84, 255));
        isCursorHand = true;
    }
    else
    {
        widgets.main[2].sprite.setTextureRect(IntRect(0, 0, 350, 80));
        message.setFillColor(Color(245, 236, 211, 255));
        isCursorHand = false;
    }
}

void changeStepString(Text &stepMessage, int move)
{
    if (move == PLAYER_MOVE)
    {
        stepMessage.setFillColor(Color(84, 84, 84, 255));
        stepMessage.setPosition(227, 598);
        stepMessage.setString("Òâîé õîä!");
    }
    else if (move == BOT_MOVE)
    {
        stepMessage.setFillColor(Color(236, 234, 210, 255));
        stepMessage.setPosition(227, 598);
        stepMessage.setString("Õîä áîòà"); 
    }
    else if (move == END_GAME)
    {
        stepMessage.setFillColor(Color(84, 84, 84, 255));
        stepMessage.setPosition(185, 598);
        stepMessage.setString("Èãðà îêîí÷åíà");
    }
}