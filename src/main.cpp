#include "gas.h"
#include <cmath>
#include <iostream>
#include <stdio.h>

const int W = 1200;
const int H = 1200;


int main () {
    sf::RenderWindow window (sf::VideoMode (W, H), "GAS EEEEEEE");
    window.setFramerateLimit (120);

    sf::Sprite sprite;
    sf::Texture texture;
    sf::Image img;
    img.create (W, H);

    Gas gas (0, 0, 600, 600);

    

    sf::Clock clk;
    double dt = 0;

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent (event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
        }

        dt = clk.restart().asSeconds();
        
        texture.loadFromImage (img);
        sprite.setTexture (texture);

        window.clear ();
        window.draw (sprite);

        window.display();
    }

    return 0;
}
