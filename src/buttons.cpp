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

void Button::SetState (double mousex, double mousey, bool mouse_pressed) {
    if (state == BTN_DISABLED) return;

    if (MouseOnButton (mousex, mousey)) {
        if (mouse_pressed) {
            if (state == BTN_NORMAL || state == BTN_FOCUSED) {
                state = BTN_PRESSED;
                Press();
            }
        }
        else {
            if (state == BTN_NORMAL)
                state = BTN_FOCUSED;
            else if (state == BTN_PRESSED) {
                state = BTN_FOCUSED;
                Release();
            }
        }
    }
    else {
        if (state == BTN_FOCUSED)
            state = BTN_NORMAL;
        else if (state == BTN_PRESSED) {
            state = BTN_NORMAL;
            Release();
        }
    }
}

bool Button::MouseOnButton (double mousex, double mousey) const {
    return (mousex >= x && mousex <= x + w && mousey >= y && mousey <= y + h);
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

void TextButton::Press() {
    std::cout << "btn pressed!\n";
}

void TextButton::Release() {
    std::cout << "btn released!\n";
}

void TextButton::Draw (sf::RenderWindow& window) {
    sf::RectangleShape rect (sf::Vector2f (w, h));
    rect.setFillColor (sf::Color (255 - state * 80, 255 - state * 80, 255 - state * 80)); // TEST
    rect.setPosition (x, y);
    text.setPosition (x, y);
    
    window.draw (rect);
    window.draw (text);
}


ButtonManager::~ButtonManager() {
    for (size_t i = 0; i < btns.size(); i++) {
        delete btns[i];
    }
}

void ButtonManager::AddButton (Button *btn) {
    btns.push_back (btn);
}

void ButtonManager::DrawButtons (sf::RenderWindow& window) {
    for (size_t i = 0; i < btns.size(); i++) {
        btns[i] -> Draw (window);
    }
}

void ButtonManager::SetStates (double mousex, double mousey, bool mouse_pressed) {
    for (size_t i = 0; i < btns.size(); i++) {
        btns[i] -> SetState (mousex, mousey, mouse_pressed);
    }
}