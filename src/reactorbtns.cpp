#include "reactorbtns.h"


ReactorBtn::ReactorBtn (double x_, double y_, double w_, double h_, Reactor* rctr_):
    ImageButton (x_, y_, w_, h_),
    rctr (rctr_)
    {}


AddCircleBtn::AddCircleBtn (double x_, double y_, double w_, double h_, Reactor* rctr_):
    ReactorBtn (x_, y_, w_, h_, rctr_)
    {}

void AddCircleBtn::Press() {
    rctr -> AddCircle (100);
}


AddSquareBtn::AddSquareBtn (double x_, double y_, double w_, double h_, Reactor* rctr_):
    ReactorBtn (x_, y_, w_, h_, rctr_)
    {}

void AddSquareBtn::Press() {
    rctr -> AddSquare (100);
}


HeatWallsBtn::HeatWallsBtn (double x_, double y_, double w_, double h_, Reactor* rctr_, double temp_change_):
    ReactorBtn (x_, y_, w_, h_, rctr_),
    temp_change (temp_change_)
    {}

void HeatWallsBtn::Release() {
    rctr -> HeatWalls (temp_change);
    std::cout << "btn works! temp changed by " << temp_change << "\n";
}
