#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>

using namespace sf;
using namespace std;

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

void displayButtonMode(Sprite *choice, Vector2i pos)
{
    for (int i = 0; i < 2; i++)
    {
        if (choice[i].getGlobalBounds().contains(pos.x, pos.y))
            choice[i].setTextureRect(IntRect(200 * i, 200, 200, 200));
        else
            choice[i].setTextureRect(IntRect(200 * i, 0, 200, 200));
    }
}

void smartStep(Init &player, Init &bot)
{
    int random;
    do
    {
        random = rand() % 9;
    } while (player.cellMode[random] || bot.cellMode[random]);
    
    bot.cellMode[random] = true;
}

int main(int argc, char *argv[])
{
    RenderWindow window(VideoMode(600, 600), "Tic-tac-toe"); // Создаем окно для игры

    Texture figures;
    figures.loadFromFile("img/figures.png");
    Sprite choice[2];
    for (int i = 0; i < 2; i++)
    {
        choice[i].setTexture(figures);
        choice[i].setPosition(50 + 300 * i, 180);
    }

    Texture bg;
    bg.loadFromFile("img/background.png");
    Sprite background(bg);

    bool isMenu = true;
    int playerCharacter = 0; // 0 - крестик, 1 - нолик
    int botCharacter = 0;
    Init player(figures), bot(figures);


    while (window.isOpen())
    {
        Vector2i mousePosition = Mouse::getPosition(window);

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (Mouse::isButtonPressed(Mouse::Left))
            {
                if (isMenu)
                {
                    for (int i = 0; i < 2; i++)
                    {
                        if (choice[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
                        {
                            playerCharacter = i;
                            botCharacter = !playerCharacter;
                            isMenu = false;
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < 9; i++)
                    {
                        if (player.cells[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
                        {
                            if (!player.cellMode[i] && !bot.cellMode[i])
                            {
                                player.cellMode[i] = true;
                                smartStep(player, bot);
                            }
                        }
                    }
                }
            }
        }

        displayButtonMode(choice, mousePosition);

        player.update(playerCharacter);
        bot.update(botCharacter);

        window.clear(Color::White);

        if (isMenu)
        {
            for (int i = 0; i < 2; i++)
                window.draw(choice[i]);
        }
        else
        {
            for (int i = 0; i < 9; i++)
                if (player.cellMode[i])
                    window.draw(player.cells[i]);
                else if (bot.cellMode[i])
                    window.draw(bot.cells[i]);

            window.draw(background);
        }

        window.display();
    }
    
    return 0;
}