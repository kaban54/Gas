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
const sf::Color     PISTON_COLOR = sf::Color (0, 128, 0);

const double MIN_REACTION_ENERGY = 320000;
const double GRAV_ACC = 100;
const int PISTON_MASS = 1000;

const double PISTON_HEIGHT = 50;
const double REACTOR_WALLS_THIKNESS = 25;

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

    void Move (double dt);

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
    int walls_temp;
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

    void HeatWalls (int temp_change);

    void AcceleratePiston (double vel_change);

    sf::Color GetWallsColor() const;

    sf::Color GetBgColor() const;

    void LockPiston();

    void UnlockPiston();
};


#endif