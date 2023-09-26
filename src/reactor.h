#ifndef REACTOR_H
#define REACTOR_H

#include "vec.h"
#include "molecules.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <assert.h>
#include <cstdlib>
#include <stdlib.h>


const sf::Color PISTON_COLOR = sf::Color (0, 128, 0);
const int PISTON_MASS = 1000;
const double PISTON_HEIGHT = 50;
const double GRAV_ACC = 100;

const double REACTOR_WALLS_THICKNESS = 25;


class Piston {
    const int mass;
    const double width;
    const double min_y;
    const double max_y;
    const double x;
    bool locked;

    public:
    const double height;
    double y;
    double vy;

    explicit Piston (double x_, double y_, double width_, double height_,
                     double min_y_, double max_y_, int mass_, double vy_ = 0);

    void Fall (double dt);

    void Move (double dy);

    void Draw (sf::RenderWindow& window) const;

    double ReflectMol (Molecule* mol);

    void Lock();

    void Unlock();
};


class Reactor {
    const double min_x;
    const double max_x;
    const double min_y;
    const double max_y;
    double walls_temp;
    Piston pist;
    double pressure;

    public:
    Gas gas;

    explicit Reactor (double min_x_, double min_y_, double max_x_, double max_y_, size_t num_of_molecules = 0);

    void Proceed (double dt);

    double GetPressure();

    double GetTemperature() const;

    size_t GetNumOfCircles() const;

    size_t GetNumOfSquares() const;

    void ReflectOffWals();

    void ReflectOffPiston();

    void Draw (sf::RenderWindow& window) const;

    void DrawWalls (sf::RenderWindow& window) const;

    void AddCircle (double vel);

    void AddSquare (double vel);

    void HeatWalls (double temp_change);

    void MovePiston (double dy);

    sf::Color GetWallsColor() const;

    sf::Color GetBgColor() const;

    void LockPiston();

    void UnlockPiston();

    void SetWallsTemp (double temp);
};


#endif