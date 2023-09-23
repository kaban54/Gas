#include "plot.h"

Plot::Plot (double x_, double y_, double w_, double h_, double max_t_, double max_f_):
    x (x_),
    y (y_),
    w (w_),
    h (h_),
    max_t (max_t_),
    max_f (max_f_),
    t_unit_size (max_t_ / 2),
    f_unit_size (max_f_ / 2),
    bg_color   (sf::Color::Black),
    plot_color (sf::Color::Red),
    axis_color (sf::Color::Green),
    txt_color  (sf::Color::Green),
    t0 (0),
    cur_t (0),
    points ()
    {}

Plot::Plot (double x_, double y_, double w_, double h_, double max_t_, double max_f_,
            double t_unit_size_, double f_unit_size_):
    x (x_),
    y (y_),
    w (w_),
    h (h_),
    max_t (max_t_),
    max_f (max_f_),
    t_unit_size (t_unit_size_),
    f_unit_size (f_unit_size_),
    bg_color   (sf::Color::Black),
    plot_color (sf::Color::Green),
    axis_color (sf::Color (64, 64, 64)),
    txt_color  (sf::Color (64, 64, 64)),
    t0 (0),
    cur_t (0),
    points ()
    {}

void Plot::Draw (sf::RenderWindow& window) {
    sf::RectangleShape rect (sf::Vector2f (w, h));
    rect.setPosition (x, y);
    rect.setFillColor (bg_color);
    window.draw (rect);

    rect.setFillColor (axis_color);
    rect.setSize (sf::Vector2f (w + 18, 6));
    rect.setPosition (x - 18, y + h);
    window.draw (rect);

    rect.setSize (sf::Vector2f (6, h + 18));
    rect.setPosition (x - 6, y);
    window.draw (rect);

    char buf [PLOT_TXTBUFSIZE] = "";
    sf::Text txt (buf, font, 20);
    txt.setFillColor (txt_color);

    rect.setSize (sf::Vector2f (6, 15));
    for (double t = 0; t <= max_t; t += t_unit_size) {
        rect.setPosition (x - 6 + t / max_t * w, y + h);
        window.draw (rect);

        snprintf (buf, 127, "%.0lf", t0 + t <= cur_t ? t0 + t : t0 + t - max_t);
        txt.setString (buf);
        txt.setPosition (x - 12 + t / max_t * w, y + h + 15);
        window.draw (txt);
    }

    rect.setSize (sf::Vector2f (15, 6));
    for (double f = 0; f <= max_f; f += f_unit_size) {
        rect.setPosition (x - 15, y + h - f / max_f * h);
        window.draw (rect);

        snprintf (buf, 127, "%.0lf", f);
        txt.setString (buf);
        txt.setPosition (x - 18 * (strlen(buf) + 1), y + h - f / max_f * h - 12);
        window.draw (txt);
    }

    sf::CircleShape pnt (2);
    pnt.setFillColor (plot_color);
    for (size_t i = 0; i < points.size(); i++) {
        double f = points[i].f;
        if (f > max_f) continue;
        double t = points[i].t - t0;
        if (t < 0) t += max_t;
        pnt.setPosition (x + w * (t / max_t) - 2, y + h * (1 - f / max_f) - 2);
        window.draw (pnt);
    }

    rect.setSize (sf::Vector2f (4, h));
    rect.setFillColor (sf::Color::White);
    rect.setPosition (x + w * (cur_t - t0) / max_t, y);
    window.draw (rect);

    txt.setString (title);
    txt.setCharacterSize (30);
    txt.setPosition (x, y - 35);
    window.draw (txt);
}

void Plot::AddPoint (double val, double dt) {
    cur_t += dt;
    if (cur_t >= t0 + max_t) t0 += max_t;

    while (!points.empty() && points.front().t + max_t <= cur_t) points.pop_front();

    points.push_back (Pair (cur_t, val));
}

void Plot::SetBGColor (const sf::Color& clr) {
    bg_color = clr;
}

void Plot::SetPlotColor (const sf::Color& clr) {
    plot_color = clr;
}

void Plot::SetAxisColor (const sf::Color& clr) {
    axis_color = clr;
}

void Plot::SetTxtColor (const sf::Color& clr) {
    txt_color = clr;
}

void Plot::SetFont (const sf::Font& fnt) {
    font = fnt;
}

void Plot::SetTitle (const char* const str) {
    strncpy (title, str, PLOT_TXTBUFSIZE - 1);
}