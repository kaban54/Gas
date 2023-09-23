#include "reactorbtns.h"


ReactorBtn::ReactorBtn (double x_, double y_, double w_, double h_, Reactor* rctr_):
    ImageButton (x_, y_, w_, h_),
    rctr (rctr_)
    {}


AddCircleBtn::AddCircleBtn (double x_, double y_, double w_, double h_, Reactor* rctr_):
    ReactorBtn (x_, y_, w_, h_, rctr_)
    {}

void AddCircleBtn::Press() {
    for (size_t i = 0; i < 10; i++) rctr -> AddCircle (400);
}


AddSquareBtn::AddSquareBtn (double x_, double y_, double w_, double h_, Reactor* rctr_):
    ReactorBtn (x_, y_, w_, h_, rctr_)
    {}

void AddSquareBtn::Press() {
    for (size_t i = 0; i < 10; i++) rctr -> AddSquare (400);
}


HeatWallsBtn::HeatWallsBtn (double x_, double y_, double w_, double h_, Reactor* rctr_, double temp_change_):
    ReactorBtn (x_, y_, w_, h_, rctr_),
    temp_change (temp_change_)
    {}

void HeatWallsBtn::Release() {
    rctr -> HeatWalls (temp_change);
}


MovePistonBtn::MovePistonBtn (double x_, double y_, double w_, double h_, Reactor* rctr_, double vel_):
    ReactorBtn (x_, y_, w_, h_, rctr_),
    vel (vel_)
    {}

void MovePistonBtn::Press() {
    rctr -> LockPiston();
}

void MovePistonBtn::Pressed (double dt) {
    rctr -> MovePiston (vel * dt);
}

void MovePistonBtn::Release() {
    rctr -> UnlockPiston();
}