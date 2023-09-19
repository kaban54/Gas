#ifndef GAS_H
#define GAS_H

#include "vec.h"
#include <SFML/Graphics.hpp>
#include <vector>

const double BASE_MOL_RADIUS = 5;

const sf::Color CIRCLE_MOL_COLOR = sf::Color::Red;
const sf::Color SQUARE_MOL_COLOR = sf::Color::Yellow;

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
    double radius;

    explicit Molecule (const Vec& pos_, const Vec& velocity_, double mass_, MoleculeTypes type_, double radius_);

    MoleculeTypes GetType () const;

    double GetEnergy () const;

    double GetMomentum () const;

    virtual void Draw (sf::RenderWindow& window) const = 0;
    
    void Move (double dt);

    double ReflectX (double min_x, double max_x);

    double ReflectY (double min_y, double max_y);
};

class CircleMol : public Molecule {
    public:
    explicit CircleMol (const Vec& pos_, const Vec& velocity_, double mass_, double radius_ = BASE_MOL_RADIUS);

    virtual void Draw (sf::RenderWindow& window) const override;
};

class SquareMol : public Molecule {
    public:
    explicit SquareMol (const Vec& pos_, const Vec& velocity_, double mass_, double radius_ = BASE_MOL_RADIUS);

    virtual void Draw (sf::RenderWindow& window) const override;
};

class Gas {
    double min_x;
    double max_x;
    double min_y;
    double max_y;

    public:
    std::vector<Molecule*> molecules;

    explicit Gas (double min_x_, double min_y_, double max_x_, double max_y_);

    ~Gas ();

    void AddMolecule (Molecule *mol);

    void MoveMolecules (double dt);

    void DrawMolecules (sf::RenderWindow& window);

    double ReflectMolecules ();

    void CollideMolecules ();
};

#endif