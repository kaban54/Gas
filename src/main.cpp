#include "reactor.h"
#include "buttons.h"
#include "reactorbtns.h"
#include "plot.h"
#include <cmath>
#include <iostream>
#include <stdio.h>

const int W = 1920;
const int H = 1080;

const char* FONT_FILENAME = "fonts/font.ttf";

void RunReactorApp();

void SetButtons (ButtonManager& btns, Reactor* rctr, sf::Texture* const textures);

void LoadBtnTextures (sf::Texture* const textures);

int main() {
    RunReactorApp();
    return 0;
}


void RunReactorApp() {
    sf::Font font;
    font.loadFromFile (FONT_FILENAME);

    Reactor rctr (70, 70, 670, 870, 50);

    ButtonManager btns;
    sf::Texture textures [24];
    LoadBtnTextures (textures);
    SetButtons (btns, &rctr, textures);

    Plot temp_graph   (930, 35 , 950, 180, 10, 10, 1, 4);
    Plot pres_graph   (930, 305, 950, 180, 10,  5, 1, 2);
    Plot circle_graph (930, 575, 950, 180, 10, 200, 1, 50);
    Plot square_graph (930, 845, 950, 180, 10, 200, 1, 50);
    temp_graph.SetPlotColor (sf::Color (255, 128, 0));
    pres_graph.SetPlotColor (sf::Color::Green);
    circle_graph.SetPlotColor (CIRCLE_MOL_COLOR);
    square_graph.SetPlotColor (SQUARE_MOL_COLOR);
    temp_graph.SetFont (font);
    pres_graph.SetFont (font);
    circle_graph.SetFont (font);
    square_graph.SetFont (font);
    temp_graph.SetTitle ("Temperature:");
    pres_graph.SetTitle ("Pressure:");
    circle_graph.SetTitle ("Number of circles:");
    square_graph.SetTitle ("Number of squares:");

    int frame_counter = 0;
    sf::Text fps_txt ("", font, 20);
    fps_txt.setPosition (0, 0);
    fps_txt.setFillColor (sf::Color::Cyan);
    char fps_str[8] = "";

    sf::RenderWindow window (sf::VideoMode (W, H), "REACTOR");
    window.setFramerateLimit (600);

    double mousex = 0;
    double mousey = 0;
    bool mouse_pressed = false;

    sf::Clock plot_clk;
    sf::Clock fps_clk;
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

        dt = clk.restart().asSeconds();

        btns.SetStates (mousex, mousey, mouse_pressed, dt);

        frame_counter++;
        if (fps_clk.getElapsedTime().asSeconds() >= 1) {
            sprintf (fps_str, "%d", frame_counter);
            fps_txt.setString (fps_str);
            fps_clk.restart();
            frame_counter = 0;
        }
        
        rctr.Proceed (dt);

        temp_graph.AddPoint (rctr.GetTemperature(), dt);
        circle_graph.AddPoint (rctr.GetNumOfCircles(), dt);
        square_graph.AddPoint (rctr.GetNumOfSquares(), dt);
        if (plot_clk.getElapsedTime().asSeconds() >= 0.2) {
            double plot_dt = plot_clk.restart().asSeconds();
            pres_graph.AddPoint (rctr.GetPressure() / plot_dt, plot_dt);
        }

        window.clear(sf::Color (128, 128, 128));
        
        rctr.Draw (window);
        
        temp_graph.Draw (window);
        pres_graph.Draw (window);
        circle_graph.Draw (window);
        square_graph.Draw (window);

        btns.DrawButtons (window);
        
        window.draw (fps_txt);
        window.display();
    }
}


void LoadBtnTextures(sf::Texture* const textures) {
    textures [0].loadFromFile ("textures/btn10.png");
    textures [1].loadFromFile ("textures/btn11.png");
    textures [2].loadFromFile ("textures/btn12.png");

    textures [4].loadFromFile ("textures/btn20.png");
    textures [5].loadFromFile ("textures/btn21.png");
    textures [6].loadFromFile ("textures/btn22.png");

    textures [8].loadFromFile ("textures/btn30.png");
    textures [9].loadFromFile ("textures/btn31.png");
    textures[10].loadFromFile ("textures/btn32.png");

    textures[12].loadFromFile ("textures/btn40.png");
    textures[13].loadFromFile ("textures/btn41.png");
    textures[14].loadFromFile ("textures/btn42.png");

    textures[16].loadFromFile ("textures/btn50.png");
    textures[17].loadFromFile ("textures/btn51.png");
    textures[18].loadFromFile ("textures/btn52.png");

    textures[20].loadFromFile ("textures/btn60.png");
    textures[21].loadFromFile ("textures/btn61.png");
    textures[22].loadFromFile ("textures/btn62.png");
}


void SetButtons (ButtonManager& btns, Reactor* rctr, sf::Texture* const textures) {
    ImageButton* btn = new AddCircleBtn (80 , 925, 100, 100, rctr);
    btn -> SetTextures (&(textures[0]), &(textures[1]), &(textures[2]), nullptr);
    btns.AddButton (btn);

    btn = new HeatWallsBtn (240, 925, 100, 100, rctr, -1);
    btn -> SetTextures (&(textures[4]), &(textures[5]), &(textures[6]), nullptr);
    btns.AddButton (btn);

    btn = new HeatWallsBtn (400, 925, 100, 100, rctr,  1);
    btn -> SetTextures (&(textures[8]), &(textures[9]), &(textures[10]), nullptr);
    btns.AddButton (btn);

    btn = new AddSquareBtn (560, 925, 100, 100, rctr);
    btn -> SetTextures (&(textures[12]), &(textures[13]), &(textures[14]), nullptr);
    btns.AddButton (btn);

    btn = new MovePistonBtn (725, 80 , 100, 100, rctr, -100);
    btn -> SetTextures (&(textures[16]), &(textures[17]), &(textures[18]), nullptr);
    btns.AddButton (btn);

    btn = new MovePistonBtn (725, 240, 100, 100, rctr,  100);
    btn -> SetTextures (&(textures[20]), &(textures[21]), &(textures[22]), nullptr);
    btns.AddButton (btn);
}