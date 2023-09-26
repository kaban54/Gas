#ifndef MOLECULES_H
#define MOLECULES_H

#include "vec.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <assert.h>
#include <cstdlib>


const double BASE_MOL_RADIUS = 5;

const sf::Color CIRCLE_MOL_COLOR = sf::Color::Red;
const sf::Color SQUARE_MOL_COLOR = sf::Color::Yellow;

const double MIN_REACTION_ENERGY = 6000;


enum MoleculeTypes {
    MOLECULE_CIRCLE = 0,
    MOLECULE_SQUARE = 1,
};

class Molecule {
    public:
    Vec pos;
    Vec velocity;
    unsigned int mass;
    double potential_energy;
    double dist_to_react;
    double radius;
    const MoleculeTypes type;

    explicit Molecule (const Vec& pos_, const Vec& velocity_, unsigned int mass_,
                       MoleculeTypes type_, double dist_to_react_);

    double GetEnergy() const;

    Vec GetMomentum() const;

    bool CanReact() const;

    virtual void Draw (sf::RenderWindow& window) const = 0;
    
    void Move (double dt);

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

    ~Gas();

    void AddMolecule (Molecule *mol);

    void RemoveMolecule (size_t index);

    void MoveMolecules (double dt);

    void DrawMolecules (sf::RenderWindow& window) const;

    void CollideMolecules();

    void React (size_t index1, size_t index2);

    void ReactCircleCircle (size_t index1, size_t index2);

    void ReactCircleSquare (size_t index1, size_t index2);

    void ReactSquareCircle (size_t index1, size_t index2);

    void ReactSquareSquare (size_t index1, size_t index2);

    double GetTemperature() const;

    size_t GetNumOfCircles() const;

    size_t GetNumOfSquares() const;
};

void ReflectMolecules (Molecule* mol1, Molecule* mol2);


#endif