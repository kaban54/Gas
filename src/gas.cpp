#include "gas.h"
#include <stdlib.h>

Molecule::Molecule (const Vec& pos_, const Vec& velocity_, double mass_, MoleculeTypes type_):
    pos (pos_),
    velocity (velocity_),
    mass (mass_),
    type (type_)
    {}

MoleculeTypes Molecule::GetType () {
    return type;
}

double Molecule::GetEnergy () {
    return mass * (velocity, velocity) / 2;
}

double Molecule::GetMomentum () {
    return mass * velocity.GetLen();
}

void Molecule::Move (double dt) {
    pos += velocity * dt;
}


CircleMol::CircleMol (const Vec& pos_, const Vec& velocity_, double mass_):
    Molecule (pos_, velocity_, mass_, MOLECULE_CIRCLE)
    {}

void CircleMol::Draw (sf::RenderWindow& window) const {
    
}


SquareMol::SquareMol (const Vec& pos_, const Vec& velocity_, double mass_):
    Molecule (pos_, velocity_, mass_, MOLECULE_SQUARE)
    {}

void SquareMol::Draw (sf::RenderWindow& window) const {

}


Gas::Gas (double min_x_, double min_y_, double max_x_, double max_y_):
    min_x (min_x_),
    max_x (max_x_),
    min_y (min_y_),
    max_y (max_y_),
    num_of_molecules (0),
    capacity (BASE_CAPACITY) {
        molecules = (Molecule **) calloc (BASE_CAPACITY, sizeof (Molecule*));
    }

Gas::~Gas () {
    for (int i = 0; i < num_of_molecules; i++) {
        delete molecules [i];
    }
    free (molecules);
}

void Gas::AddMolecule (Molecule *mol) {
    if (num_of_molecules >= capacity) {
        molecules = (Molecule**) realloc (molecules, (capacity *= 2) * sizeof (Molecule*));
    }
    molecules [num_of_molecules++] = mol;
}

void Gas::DrawMolecules (sf::RenderWindow& window) {
    for (int i = 0; i < num_of_molecules; i++) {
        molecules[i] -> Draw (window);
    }
}

void Gas::MoveMolecules (double dt) {
    for (int i = 0; i < num_of_molecules; i++) {
        molecules [i] -> Move (dt);
        // Reflection
    }
    // Collisions
}