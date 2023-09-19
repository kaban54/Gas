#include "gas.h"
#include <stdlib.h>

Molecule::Molecule (const Vec& pos_, const Vec& velocity_, double mass_, MoleculeTypes type_, double radius_):
    pos (pos_),
    velocity (velocity_),
    mass (mass_),
    type (type_),
    radius (radius_)
    {}

MoleculeTypes Molecule::GetType () const {
    return type;
}

double Molecule::GetEnergy () const {
    return mass * (velocity, velocity) / 2;
}

double Molecule::GetMomentum () const {
    return mass * velocity.GetLen();
}

void Molecule::Move (double dt) {
    pos += velocity * dt;
}

double Molecule::ReflectX (double min_x, double max_x) {
    if ((pos.x - radius <= min_x && velocity.x < 0) ||
        (pos.x + radius >= max_x && velocity.x > 0)) {
        velocity.x = -velocity.x;
        return velocity.x * mass;
    }
    else return 0;
}

double Molecule::ReflectY (double min_y, double max_y) {
    if ((pos.y - radius <= min_y && velocity.y < 0) ||
        (pos.y + radius >= max_y && velocity.y > 0)) {
        velocity.y = -velocity.y;
        return velocity.y * mass;
    }
    else return 0;
}


CircleMol::CircleMol (const Vec& pos_, const Vec& velocity_, double mass_, double radius_):
    Molecule (pos_, velocity_, mass_, MOLECULE_CIRCLE, radius_)
    {}

void CircleMol::Draw (sf::RenderWindow& window) const {
    sf::CircleShape circle (radius); // change to sprite
    circle.setPosition (pos.x - radius, pos.y - radius);
    circle.setFillColor (CIRCLE_MOL_COLOR);
    window.draw (circle);
}


SquareMol::SquareMol (const Vec& pos_, const Vec& velocity_, double mass_, double radius_):
    Molecule (pos_, velocity_, mass_, MOLECULE_SQUARE, radius_)
    {}

void SquareMol::Draw (sf::RenderWindow& window) const {
    sf::RectangleShape square (sf::Vector2f(radius * 2, radius * 2)); // change to sprite
    square.setPosition (pos.x - radius, pos.y - radius);
    square.setFillColor (SQUARE_MOL_COLOR);
    window.draw (square);
}


Gas::Gas (double min_x_, double min_y_, double max_x_, double max_y_):
    min_x (min_x_),
    max_x (max_x_),
    min_y (min_y_),
    max_y (max_y_),
    molecules ()
    {}

Gas::~Gas () {
    for (int i = 0; i < molecules.size(); i++) {
        delete molecules[i];
    }
}

void Gas::AddMolecule (Molecule *mol) {
    molecules.push_back (mol);
}

void Gas::DrawMolecules (sf::RenderWindow& window) {
    for (size_t i = 0; i < molecules.size(); i++) {
        molecules[i] -> Draw (window);
    }
}

void Gas::MoveMolecules (double dt) {
    for (int i = 0; i < molecules.size(); i++) {
        molecules[i] -> Move (dt);
    }
}

double Gas::ReflectMolecules () {
    double pressure = 0;
    for (int i = 0; i < molecules.size(); i++) {
        pressure += molecules[i] -> ReflectX (min_x, max_x);
        pressure += molecules[i] -> ReflectY (min_y, max_y);
    }
    return pressure;
}