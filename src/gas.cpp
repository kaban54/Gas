#include "gas.h"
#include <stdlib.h>

Molecule::Molecule (const Vec& pos_, const Vec& velocity_, unsigned int mass_,
                    MoleculeTypes type_, double dist_to_react_):
    pos (pos_),
    velocity (velocity_),
    mass (mass_),
    type (type_),
    dist_to_react (dist_to_react_),
    radius (BASE_MOL_RADIUS + mass_)
    {}

double Molecule::GetEnergy () const {
    return mass * (velocity, velocity);
}

double Molecule::GetMomentum () const {
    return mass * velocity.GetLen();
}

bool Molecule::CanReact () const {
    return dist_to_react <= 0;
}

void Molecule::Move (double dt) {
    pos += velocity * dt;
    dist_to_react -= velocity.GetLen() * dt;
}

void Molecule::SetMass (unsigned int new_mass) {
    mass = new_mass;
    radius = BASE_MOL_RADIUS + mass;
}


CircleMol::CircleMol (const Vec& pos_, const Vec& velocity_, unsigned int mass_, double dist_to_react_):
    Molecule (pos_, velocity_, mass_, MOLECULE_CIRCLE, dist_to_react_)
    {}

void CircleMol::Draw (sf::RenderWindow& window) const {
    sf::CircleShape circle (radius); // change to sprite
    circle.setPosition (pos.x - radius, pos.y - radius);
    circle.setFillColor (CIRCLE_MOL_COLOR);
    window.draw (circle);
}


SquareMol::SquareMol (const Vec& pos_, const Vec& velocity_, unsigned int mass_, double dist_to_react_):
    Molecule (pos_, velocity_, mass_, MOLECULE_SQUARE, dist_to_react_)
    {}

void SquareMol::Draw (sf::RenderWindow& window) const {
    sf::RectangleShape square (sf::Vector2f(radius * 2, radius * 2)); // change to sprite
    square.setPosition (pos.x - radius, pos.y - radius);
    square.setFillColor (SQUARE_MOL_COLOR);
    window.draw (square);
}

bool Intersect (Molecule* mol1, Molecule* mol2) {
    if ((mol1 -> pos - mol2 -> pos).GetLen() <= mol1 -> radius + mol2 -> radius) return true;
    else return false;
}


Gas::Gas ():
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

void Gas::RemoveMolecule (size_t index) {
    delete molecules[index];
    molecules[index] = molecules[molecules.size() - 1];
    molecules.pop_back();
}

void Gas::DrawMolecules (sf::RenderWindow& window) const {
    for (size_t i = 0; i < molecules.size(); i++) {
        molecules[i] -> Draw (window);
    }
}

void Gas::MoveMolecules (double dt) {
    for (int i = 0; i < molecules.size(); i++) {
        molecules[i] -> Move (dt);
    }
}

void Gas::CollideMolecules () {
    for (size_t i = 0; i < molecules.size(); i++) {
        if (!(molecules[i] -> CanReact())) continue;
        for (size_t j = i + 1; j < molecules.size(); j++) {
            if (!(molecules[j] -> CanReact())) continue;
            if (Intersect (molecules[i], molecules[j])) {
                if (molecules[i] -> GetEnergy() + molecules[j] -> GetEnergy() >= MIN_REACTION_ENERGY)
                    React (i, j);
                else
                    ReflectMolecules (molecules[i], molecules[j]);
            }
        }
    }
}

void Gas::React (size_t index1, size_t index2) {
    typedef void (Gas::*ReactFunc) (size_t index1, size_t index2);
    static const ReactFunc ReactFuncTable [2][2] = 
        {{&Gas::ReactCircleCircle, &Gas::ReactCircleSquare},
         {&Gas::ReactSquareCircle, &Gas::ReactSquareSquare}};
    
    MoleculeTypes type1 = molecules[index1] -> type;
    MoleculeTypes type2 = molecules[index2] -> type;

    assert (0 <= type1 && type1 <= 1);
    assert (0 <= type2 && type2 <= 1);

    ReactFunc react = ReactFuncTable[type1][type2];

    (this ->* react) (index1, index2);
}

void Gas::ReactCircleCircle (size_t index1, size_t index2) {
    Molecule* mol1 = molecules[index1];
    Molecule* mol2 = molecules[index2];

    Vec newpos = (mol1 -> pos + mol2 -> pos) / 2;
    int newmass = mol1 -> mass + mol2 -> mass;

    Vec newvel (std::sqrt((mol1 -> GetEnergy () + mol2 -> GetEnergy ()) / newmass), 0);
    newvel.RotateAroundZ (GetRandAngle());

    Molecule* newmol = new SquareMol (newpos, newvel, newmass);

    AddMolecule (newmol);
    RemoveMolecule (index2);
    RemoveMolecule (index1);
}

void Gas::ReactCircleSquare (size_t index1, size_t index2) {
    ReactSquareCircle (index2, index1);
}

void Gas::ReactSquareCircle (size_t index1, size_t index2) {
    Molecule* mol1 = molecules[index1];
    Molecule* mol2 = molecules[index2];

    int newmass = mol1 -> mass + mol2 -> mass;

    Vec newvel = !(mol1 -> velocity) * std::sqrt((mol1 -> GetEnergy () + mol2 -> GetEnergy ()) / newmass);

    mol1 -> velocity = newvel;
    mol1 -> SetMass (newmass);

    RemoveMolecule (index2);
}

void Gas::ReactSquareSquare (size_t index1, size_t index2) {
    Molecule* mol1 = molecules[index1];
    Molecule* mol2 = molecules[index2];

    int num_of_new = mol1 -> mass + mol2 -> mass;
    Vec pos = (mol1 -> pos + mol2 -> pos) / 2;

    Vec vel (std::sqrt((mol1 -> GetEnergy () + mol2 -> GetEnergy ()) / num_of_new), 0);

    double angle = 2 * M_PI / num_of_new;
    vel.RotateAroundZ (GetRandAngle());

    for (int i = 0; i < num_of_new; i++) {
        Molecule *mol = new CircleMol (pos, vel, 1, 30 + num_of_new * (BASE_MOL_RADIUS + 1) / 3);
        AddMolecule (mol);
        vel.RotateAroundZ (angle);
    }

    RemoveMolecule (index2);
    RemoveMolecule (index1);
}

double Gas::GetTemperature() const {
    double energy = 0;
    for (size_t i = 0; i < molecules.size(); i++)
        energy += molecules[i] -> GetEnergy();
    
    return energy / molecules.size();
}

void ReflectMolecules (Molecule* mol1, Molecule* mol2) {
    Vec dif = mol1 -> pos - mol2 -> pos;
    dif.Normalize();

    double v1 = (mol1 -> velocity, dif);
    double v2 = (mol2 -> velocity, dif);
    double k = mol2 -> mass / mol1 -> mass;

    double new_v2 = (2 * v1 + v2 * (k - 1)) / (k + 1);
    double new_v1 = new_v2 + v2 - v1;
    
    mol1 -> velocity += (dif * (new_v1 - v1));
    mol2 -> velocity += (dif * (new_v2 - v2));
}


Piston::Piston (double x_, double y_, double width_, double height_,
                double min_y_, double max_y_, int mass_, double vy_):
    x (x_),
    y (y_),
    width (width_),
    height (height_),
    min_y (min_y_),
    max_y (max_y_ - height_),
    mass (mass_),
    vy (vy_)
    {}

void Piston::Move (double dt) {
    if (y > max_y) {
        y = max_y;
        vy = 0;
    }
    if (y < min_y) {
        y = min_y;
        vy = 0;
    }
    vy += GRAV_ACC * dt;
    y += vy * dt;
}

void Piston::Draw (sf::RenderWindow& window) const {
    sf::RectangleShape rect (sf::Vector2f(width, height));
    rect.setPosition (x, y);
    rect.setFillColor (PISTON_COLOR);
    window.draw (rect);
}

void Piston::ReflectMol (Molecule* mol) {
    double v1 = mol -> velocity.y;
    double v2 = vy;
    double k = mass / mol -> mass;

    vy = (2 * v1 + v2 * (k - 1)) / (k + 1);
    double new_v1 = vy + v2 - v1;
    
    mol -> velocity.y = new_v1;
    mol -> pos.y = y + height + mol -> radius;
}


Reactor::Reactor (double min_x_, double min_y_, double max_x_, double max_y_, size_t num_of_molecules):
    min_x (min_x_),
    min_y (min_y_),
    max_x (max_x_),
    max_y (max_y_),
    walls_temp (0),
    pressure (0),
    pist (min_x_, min_y_, max_x_ - min_x_, PISTON_HEIGHT, min_y_, max_y_, PISTON_MASS),
    gas ()
    {
        for (size_t i = 0; i < num_of_molecules; i++) {
            
            Vec pos (min_x + std::rand() / (RAND_MAX / (max_x - min_x)),
                     min_y + std::rand() / (RAND_MAX / (max_y - min_y)));

            Vec vel (std::rand() / (RAND_MAX / 400.), 0);
            vel.RotateAroundZ (GetRandAngle());

            Molecule* mol = nullptr;
            if (i % 2 == 0) mol = new CircleMol (pos, vel, 1);
            else            mol = new SquareMol (pos, vel, 1);
            gas.AddMolecule (mol);
        }
    }

void Reactor::Proceed (double dt) {
    gas.MoveMolecules (dt);
    pist.Move (dt);
    gas.CollideMolecules();
    ReflectOffWals();
    ReflectOffPiston();
}

double Reactor::GetPressure() {
    double ret = pressure;
    pressure = 0;
    return ret;
}

double Reactor::GetTemperature() const {
    return gas.GetTemperature();
}

void Reactor::ReflectOffWals() {
    for (size_t i = 0; i < gas.molecules.size(); i++) {
        Molecule* mol = gas.molecules[i];

        if (mol -> pos.x + mol -> radius >= max_x) {
            mol -> pos.x = max_x - mol -> radius;
            if (mol -> velocity.x > 0) {
                mol -> velocity.x = -mol -> velocity.x;
                pressure += 2 * mol -> mass * fabs(mol -> velocity.x);
            }
        }
        if (mol -> pos.x - mol -> radius <= min_x) {
            mol -> pos.x = min_x + mol -> radius;
            if (mol -> velocity.x < 0) {
                mol -> velocity.x = -mol -> velocity.x;
                pressure += 2 * mol -> mass * fabs(mol -> velocity.x);
            }
        }
        if (mol -> pos.y + mol -> radius >= max_y) {
            mol -> pos.y = max_y - mol -> radius;
            if (mol -> velocity.y > 0) {
                mol -> velocity.y = -mol -> velocity.y;
                pressure += 2 * mol -> mass * fabs(mol -> velocity.y);
            }
        }
        if (mol -> pos.y - mol -> radius <= min_y) {
            mol -> pos.y = min_y + mol -> radius;
            if (mol -> velocity.y < 0) {
                mol -> velocity.y = -mol -> velocity.y;
                pressure += 2 * mol -> mass * fabs(mol -> velocity.y);
            }
        }
    }
}

void Reactor::ReflectOffPiston() {
    for (size_t i = 0; i < gas.molecules.size(); i++) {
        Molecule* mol = gas.molecules[i];
        if (mol -> pos.y - mol -> radius <= pist.y + pist.height)
            pist.ReflectMol (mol);
    }
}

void Reactor::Draw (sf::RenderWindow& window) const {
    DrawWalls (window);
    pist.Draw(window);
    gas.DrawMolecules(window);
}

void Reactor::DrawWalls (sf::RenderWindow& window) const {
    sf::RectangleShape box (sf::Vector2f(max_x - min_x, max_y - min_y));
    box.setPosition (min_x, min_y);
    box.setOutlineColor (sf::Color::White);
    box.setFillColor (sf::Color::Black);
    box.setOutlineThickness (REACTOR_WALLS_THIKNESS);
    window.draw (box);
}

void Reactor::AddCircle (double vel) {
    Vec velocity (vel, 0);
    velocity.RotateAroundZ (GetRandAngle() / -4);
    Molecule* mol = new CircleMol (Vec (min_x, max_y), velocity, 1);
    gas.AddMolecule (mol);
}

void Reactor::AddSquare (double vel) {
    Vec velocity (vel, 0);
    velocity.RotateAroundZ (GetRandAngle() / 4 + M_PI);
    Molecule* mol = new SquareMol (Vec (max_x, max_y), velocity, 1);
    gas.AddMolecule (mol);
}