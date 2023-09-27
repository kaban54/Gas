#include "molecules.h"

Molecule::Molecule (const Vec& pos_, const Vec& velocity_, unsigned int mass_,
                    MoleculeTypes type_, double dist_to_react_):
    pos (pos_),
    velocity (velocity_),
    mass (mass_),
    potential_energy (0),
    type (type_),
    dist_to_react (dist_to_react_),
    radius (BASE_MOL_RADIUS + mass_)
    {}

double Molecule::GetKineticEnergy() const {
    return mass * (velocity, velocity);
}

double Molecule::GetEnergy() const {
    return GetKineticEnergy() + potential_energy;
}

Vec Molecule::GetMomentum() const {
    return mass * velocity;
}

bool Molecule::CanReact() const {
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
    sf::CircleShape circle (radius);
    circle.setPosition (pos.x - radius, pos.y - radius);
    circle.setFillColor (CIRCLE_MOL_COLOR);
    window.draw (circle);
}


SquareMol::SquareMol (const Vec& pos_, const Vec& velocity_, unsigned int mass_, double dist_to_react_):
    Molecule (pos_, velocity_, mass_, MOLECULE_SQUARE, dist_to_react_)
    {}

void SquareMol::Draw (sf::RenderWindow& window) const {
    sf::RectangleShape square (sf::Vector2f(radius * 2, radius * 2));
    square.setPosition (pos.x - radius, pos.y - radius);
    square.setFillColor (SQUARE_MOL_COLOR);
    window.draw (square);
}

bool Intersect (Molecule* mol1, Molecule* mol2) {
    if ((mol1 -> pos - mol2 -> pos).GetLen() <= mol1 -> radius + mol2 -> radius) return true;
    else return false;
}

Gas::Gas() {
    size = 0;
    capacity = BASE_GAS_CAPACITY;
    molecules = (Molecule**) calloc (capacity, sizeof (molecules[0]));
    assert (molecules != nullptr);
}

Gas::~Gas() {
    for (size_t i = 0; i < size; i++) {
        delete molecules[i];
    }
    free (molecules);
}

void Gas::AddMolecule (Molecule *mol) {
    if (size >= capacity) {
        molecules = (Molecule**) Recalloc (molecules, capacity * 2, sizeof (molecules[0]), capacity);
        assert (molecules != nullptr);
        capacity *= 2;
    }
    molecules [size++] = mol;
}

void Gas::RemoveMolecule (size_t index) {
    delete molecules[index];
    molecules[index] = molecules[--size];
}

void Gas::DrawMolecules (sf::RenderWindow& window) const {
    for (size_t i = 0; i < size; i++) {
        molecules[i] -> Draw (window);
    }
}

void Gas::MoveMolecules (double dt) {
    for (size_t i = 0; i < size; i++) {
        molecules[i] -> Move (dt);
    }
}

void Gas::CollideMolecules() {
    for (size_t i = 0; i < size; i++) {
        if (!(molecules[i] -> CanReact())) continue;
        for (size_t j = i + 1; j < size; j++) {
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

    Vec newvel = (mol1 -> GetMomentum() + mol2 -> GetMomentum()) / newmass;

    Molecule* newmol = new SquareMol (newpos, newvel, newmass);

    newmol -> potential_energy += mol1 -> GetKineticEnergy() + mol2 -> GetKineticEnergy() - newmass * (newvel, newvel);

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

    Vec newvel = (mol1 -> GetMomentum() + mol2 -> GetMomentum()) / newmass;

    mol1 -> potential_energy += mol1 -> GetKineticEnergy() + mol2 -> GetKineticEnergy() - newmass * (newvel, newvel);

    mol1 -> velocity = newvel;
    mol1 -> SetMass (newmass);

    RemoveMolecule (index2);
}

void Gas::ReactSquareSquare (size_t index1, size_t index2) {
    Molecule* mol1 = molecules[index1];
    Molecule* mol2 = molecules[index2];

    int num_of_new = mol1 -> mass + mol2 -> mass;
    Vec pos = (mol1 -> pos + mol2 -> pos) / 2;

    Vec vel (std::sqrt((mol1 -> GetEnergy() + mol2 -> GetEnergy()) / num_of_new), 0);

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
    for (size_t i = 0; i < size; i++)
        energy += molecules[i] -> GetKineticEnergy();
    
    return energy / size / 1e6;
}

size_t Gas::GetNumOfCircles() const {
    size_t count = 0;
    for (size_t i = 0; i < size; i++)
        if (molecules[i] -> type == MOLECULE_CIRCLE) count++;
    return count;
}

size_t Gas::GetNumOfSquares() const {
    size_t count = 0;
    for (size_t i = 0; i < size; i++)
        if (molecules[i] -> type == MOLECULE_SQUARE) count++;
    return count;
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

void *Recalloc (void *memptr, size_t num, size_t size, size_t old_num) {
    memptr = realloc (memptr, num * size);
    if (memptr == nullptr) return nullptr;

    if (num > old_num) memset ((void *) ((char *) memptr + old_num * size), 0, (num - old_num) * size);

    return memptr;
}