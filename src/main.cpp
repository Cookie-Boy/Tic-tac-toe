#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>

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
            cells[i].setTextureRect(IntRect(200 * (figure - 1), 0, 200, 200));
    }
};

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

    int result = 0; // 1 - крестик, 2 - нолик
    Init player(figures);


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
                if (result == 0)
                {
                    for (int i = 0; i < 2; i++)
                    {
                        if (choice[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
                            result = i + 1;
                    }
                }
                else
                {
                    for (int i = 0; i < 9; i++)
                    {
                        if (player.cells[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
                            player.cellMode[i] = true;
                    }
                }
            }
        }

        for (int i = 0; i < 2; i++)
        {
            if (choice[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
                choice[i].setTextureRect(IntRect(200 * i, 200, 200, 200));
            else
                choice[i].setTextureRect(IntRect(200 * i, 0, 200, 200));
        }

        player.update(result);
        window.clear(Color::White);

        if (result == 0)
        {
            for (int i = 0; i < 2; i++)
                window.draw(choice[i]);
        }
        else
        {
            for (int i = 0; i < 9; i++)
                if (player.cellMode[i])
                    window.draw(player.cells[i]);

            window.draw(background);
        }

        window.display();
    }
    
    return 0;
}