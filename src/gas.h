#ifndef GAS_H
#define GAS_H

#include "vec.h"
#include <SFML/Graphics.hpp>

const size_t BASE_CAPACITY = 8;

const double CIRCLE_MOL_RADIUS = 5;
const double SQUARE_MOL_SIDE = 10;

enum MoleculeTypes {
    MOLECULE_CIRCLE = 1,
    MOLECULE_SQUARE = 2,
};

class Molecule {
    private:
    MoleculeTypes type;

    public:
    Vec pos;
    Vec velocity;
    double mass;

    explicit Molecule (const Vec& pos_, const Vec& velocity_, double mass_, MoleculeTypes type_);

    MoleculeTypes GetType ();

    double GetEnergy ();

    double GetMomentum ();

    virtual void Draw (sf::RenderWindow& window) const = 0;
    
    void Move (double dt);
};

class CircleMol : public Molecule {
    public:
    explicit CircleMol (const Vec& pos_, const Vec& velocity_, double mass_);

    virtual void Draw (sf::RenderWindow& window) const override;
};

class SquareMol : public Molecule {
    public:
    explicit SquareMol (const Vec& pos_, const Vec& velocity_, double mass_);

    virtual void Draw (sf::RenderWindow& window) const override;
};

class Gas {
    double min_x;
    double max_x;
    double min_y;
    double max_y;
    size_t num_of_molecules;
    size_t capacity;

    public:
    Molecule** molecules;

    explicit Gas (double min_x_, double min_y_, double max_x_, double max_y_);

    ~Gas ();

    void AddMolecule (Molecule *mol);

    void MoveMolecules (double dt);

    void DrawMolecules (sf::RenderWindow& window);
};

#endif