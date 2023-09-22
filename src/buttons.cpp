#include "buttons.h"

Button::Button (double x_, double y_, double w_, double h_):
    x (x_),
    y (y_),
    w (w_),
    h (h_),
    state (BTN_NORMAL)
    {}

Button::Button():
    x (0),
    y (0),
    w (100),
    h (100),
    state (BTN_NORMAL)
    {}

void Button::SetPos (double x_, double y_) {
    x = x_;
    y = y_;
}

void Button::SetSize (double w_, double h_) {
    w = w_;
    h = h_;
}



TextButton::TextButton (double x_, double y_, double w_, double h_, const sf::Font& font, const char* str):
    Button (x_, y_, w_, h_),
    text (str, font)
    {
        text.setFillColor (sf::Color::Black);
    }

void TextButton::SetText (const char* str) {
    text.setString (str);
}

void TextButton::SetFont (const sf::Font& font) {
    text.setFont (font);
}

void TextButton::Draw (sf::RenderWindow& window) {
    sf::RectangleShape rect (sf::Vector2f (w, h));
    rect.setFillColor (sf::Color (255 - state * 80, 255 - state * 80, 255 - state * 80)); // TEST
    rect.setPosition (x, y);
    text.setPosition (x, y);
    
    window.draw (rect);
    window.draw (text);
}