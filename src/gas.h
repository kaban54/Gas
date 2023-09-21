#ifndef GAS_H
#define GAS_H

#include "vec.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <assert.h>
#include <cstdlib>

const double BASE_MOL_RADIUS = 5;

const sf::Color CIRCLE_MOL_COLOR = sf::Color::Red;
const sf::Color SQUARE_MOL_COLOR = sf::Color::Yellow;

const double MIN_REACTION_ENERGY = 320000;

enum MoleculeTypes {
    MOLECULE_CIRCLE = 0,
    MOLECULE_SQUARE = 1,
};

class Molecule {
    public:
    Vec pos;
    Vec velocity;
    unsigned int mass;
    double dist_to_react;
    double radius;
    const MoleculeTypes type;

    explicit Molecule (const Vec& pos_, const Vec& velocity_, unsigned int mass_,
                       MoleculeTypes type_, double dist_to_react_);

    double GetEnergy() const;

    double GetMomentum() const;

    bool CanReact() const;

    virtual void Draw (sf::RenderWindow& window) const = 0;
    
    void Move (double dt);

    double ReflectX (double min_x, double max_x);

    double ReflectY (double min_y, double max_y);

    void SetMass (unsigned int new_mass);
};


class CircleMol : public Molecule {
    public:
    explicit CircleMol (const Vec& pos_, const Vec& velocity_, unsigned int mass_,
                        double dist_to_react_ = 0);

    virtual void Draw (sf::RenderWindow& window) const override;
};

class SquareMol : public Molecule {
    public:
    explicit SquareMol (const Vec& pos_, const Vec& velocity_, unsigned int mass_,
                        double dist_to_react_ = 0);

    virtual void Draw (sf::RenderWindow& window) const override;
};

bool Intersect (Molecule* mol1, Molecule* mol2);


class Gas {
    public:
    std::vector<Molecule*> molecules;

    explicit Gas ();

    ~Gas();

    void AddMolecule (Molecule *mol);

    void RemoveMolecule (size_t index);

    void MoveMolecules (double dt);

    void DrawMolecules (sf::RenderWindow& window) const;

    //double ReflectMolecules();

    void CollideMolecules();

    void React (size_t index1, size_t index2);

    void ReactCircleCircle (size_t index1, size_t index2);

    void ReactCircleSquare (size_t index1, size_t index2);

    void ReactSquareCircle (size_t index1, size_t index2);

    void ReactSquareSquare (size_t index1, size_t index2);
};


class Reactor {
    double min_x;
    double max_x;
    double min_y;
    double max_y;
    double walls_temp;

    public:
    Gas gas;

    explicit Reactor (double min_x_, double min_y_, double max_x_, double max_y_);

    void Proceed (double dt);

    double GetPressure() const;

    double GetTemperature() const;

    void ReflectOffWals();

    void Draw (sf::RenderWindow& window) const;

    void DrawWalls (sf::RenderWindow& window) const;

    void AddCircle (double vel);

    void AddSquare (double vel);
};


void ReflectMols (Molecule* mol1, Molecule* mol2);

#endif