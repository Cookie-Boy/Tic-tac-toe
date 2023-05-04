#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

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

    while (window.isOpen())
    {
        Vector2i mousePosition = Mouse::getPosition(window);

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        for (int i = 0; i < 2; i++)
        {
            if (choice[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
                choice[i].setTextureRect(IntRect(200 * i, 200, 200, 200));
            else
                choice[i].setTextureRect(IntRect(200 * i, 0, 200, 200));
        }

        window.clear(Color::White);
        for (int i = 0; i < 2; i++)
            window.draw(choice[i]);
        window.display();
    }
    
    return 0;
}