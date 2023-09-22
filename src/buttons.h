#ifndef BUTTONS_H
#define BUTTONS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <assert.h>
#include <cstdlib>
#include <string>

const size_t TXTBTN_BUFSIZE = 128;

enum ButtonState {
    BTN_NORMAL = 0,
    BTN_PRESSED = 1,
    BTN_FOCUSED = 2,
    BTN_DISABLED = 3,
};


class Button {
    public:

    double x;
    double y;
    double w;
    double h;
    ButtonState state;

    explicit Button (double x_, double y_, double w_, double h_);

    explicit Button ();

    void SetPos (double x_, double y_);

    void SetSize (double w_, double h_);

    virtual void Draw (sf::RenderWindow& window) = 0;
};


class TextButton : public Button {
    sf::Text text;

    public:

    explicit TextButton (double x_, double y_, double w_, double h_, const sf::Font& font, const char* str = "");

    void SetText (const char* str);

    void SetFont (const sf::Font& font);

    virtual void Draw (sf::RenderWindow& window) override;
};


#endif