#include "gas.h"
#include "buttons.h"
#include "reactorbtns.h"
#include <cmath>
#include <iostream>
#include <stdio.h>

const int W = 1920;
const int H = 1080;

const char* FONT_FILENAME = "fonts/font.ttf";

void RunReactorApp();

int main() {
    RunReactorApp();
    return 0;
}


void RunReactorApp() {

    sf::Texture test_btn_texture;
    test_btn_texture.loadFromFile ("images/suhariki.jpg");

    sf::Font font;
    font.loadFromFile (FONT_FILENAME);

    sf::Text fps_txt ("", font, 20);
    fps_txt.setPosition (0, 0);
    fps_txt.setFillColor (sf::Color::Cyan);
    char fps_str[8] = "";

    Reactor rctr (70, 70, 670, 870, 0);

    ButtonManager btns;
    btns.AddButton (new AddCircleBtn (80 , 925, 100, 100, &rctr));
    btns.AddButton (new HeatWallsBtn (240, 925, 100, 100, &rctr, -100));
    btns.AddButton (new HeatWallsBtn (400, 925, 100, 100, &rctr,  100));
    btns.AddButton (new AddSquareBtn (560, 925, 100, 100, &rctr));
    btns.AddButton (new AcceleratePistonBtn (725, 80 , 100, 100, &rctr, -100));
    btns.AddButton (new AcceleratePistonBtn (725, 240, 100, 100, &rctr,  100));

    sf::Clock clk;
    double dt = 0;

    sf::RenderWindow window (sf::VideoMode (W, H), "GAS EEEEEEE");
    window.setFramerateLimit (600);

    double mousex = 0;
    double mousey = 0;
    bool mouse_pressed = false;

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

            if (event.type == sf::Event::MouseMoved) {
                mousex = event.mouseMove.x;
                mousey = event.mouseMove.y;
            }

            if (event.type == sf::Event::MouseButtonPressed)
                mouse_pressed = true;

            if (event.type == sf::Event::MouseButtonReleased)
                mouse_pressed = false;
        }

        btns.SetStates (mousex, mousey, mouse_pressed);

        dt = clk.restart().asSeconds();

        sprintf (fps_str, "%.0f", 1 / dt);
        fps_txt.setString (fps_str);
        
        rctr.Proceed (dt);

        window.clear(sf::Color (192, 192, 192));
        rctr.Draw (window);
        btns.DrawButtons (window);
        window.draw (fps_txt);
        window.display();
    }
}
