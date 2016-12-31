#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <time.h>
#include "Game.hpp"

int main()
{
    sf::RenderWindow app(sf::VideoMode(1024, 768, 32), "Meuporg", sf::Style::Close);
    app.setFramerateLimit(60);

    srand(time(NULL));

    Game game(app);

    game.loop();

    return EXIT_SUCCESS;
}
