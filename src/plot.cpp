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
    sf::RectangleShape bg (sf::Vector2f (w, h));
    bg.setPosition (x, y);
    bg.setFillColor (bg_color);
    window.draw (bg);

    sf::RectangleShape taxis (sf::Vector2f (w + 18, 6));
    taxis.setPosition (x - 18, y + h);
    taxis.setFillColor (axis_color);
    window.draw (taxis);

    sf::RectangleShape faxis (sf::Vector2f (6, h + 18));
    faxis.setPosition (x - 6, y);
    faxis.setFillColor (axis_color);
    window.draw (faxis);

    sf::RectangleShape tmark (sf::Vector2f (6, 18));
    tmark.setFillColor (axis_color);
    for (double t = 0; t <= max_t; t += t_unit_size) {
        tmark.setPosition (x - 6 + t / max_t * w, y + h);
        window.draw (tmark);
    }

    sf::RectangleShape fmark (sf::Vector2f (18, 6));
    fmark.setFillColor (axis_color);
    for (double f = 0; f <= max_f; f += f_unit_size) {
        fmark.setPosition (x - 18, y + h - f / max_f * h);
        window.draw (fmark);
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

    sf::RectangleShape sep (sf::Vector2f (4, h));
    sep.setFillColor (sf::Color::White);
    sep.setPosition (x + w * (cur_t - t0) / max_t, y);
    window.draw (sep);
}

void Plot::AddPoint (double val, double dt) {
    cur_t += dt;
    if (cur_t >= t0 + max_t) t0 += max_t;

    while (!points.empty() && points.front().t + max_t <= cur_t) points.pop_front();

    points.push_back (Pair (cur_t, val));

    //std::cout << "cur_t = " << cur_t << "; t0 = " << t0 << ";\n";
}