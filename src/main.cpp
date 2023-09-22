#include "gas.h"
#include "buttons.h"
#include <cmath>
#include <iostream>
#include <stdio.h>

const int W = 1920;
const int H = 1080;

const char* FONT_FILENAME = "fonts/font.ttf";

void RunReactorApp ();

int main () {
    RunReactorApp ();
    return 0;
}


void RunReactorApp () {
    sf::Font font;
    font.loadFromFile(FONT_FILENAME);

    sf::Text fps_txt ("", font, 20);
    fps_txt.setPosition (0, 0);
    fps_txt.setFillColor (sf::Color::Cyan);
    char fps_str [8] = "";

    Reactor rctr (50, 50, 650, 850, 100);


    

    TextButton test_btn (1000, 800, 200, 200, font, "ABCD");


    sf::Clock clk;
    double dt = 0;

    sf::RenderWindow window (sf::VideoMode (W, H), "GAS EEEEEEE");
    window.setFramerateLimit (600);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent (event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                if (event.key.code == sf::Keyboard::Q)
                    rctr.AddCircle (400);
                if (event.key.code == sf::Keyboard::W)
                    rctr.AddSquare (400);
            }
        }

        dt = clk.restart().asSeconds();

        sprintf (fps_str, "%.0f", 1 / dt);
        fps_txt.setString (fps_str);
        
        rctr.Proceed (dt);

        window.clear();
        rctr.Draw (window);
        window.draw (fps_txt);
        test_btn.Draw (window);
        window.display();
    }
}