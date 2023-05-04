#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

int main(int argc, char *argv[])
{
    RenderWindow window(VideoMode(600, 600), "Tic-tac-toe"); // Создаем окно для игры

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear(Color::White);
        window.display();
    }
    
    return 0;
}