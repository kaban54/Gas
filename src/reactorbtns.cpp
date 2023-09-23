#include "reactorbtns.h"

AddCircleBtn::AddCircleBtn (double x_, double y_, double w_, double h_, Reactor* rctr_):
    ImageButton (x_, y_, w_, h_),
    rctr (rctr_)
    {}

void AddCircleBtn::Press() {
    rctr -> AddCircle (100);
}

AddSquareBtn::AddSquareBtn (double x_, double y_, double w_, double h_, Reactor* rctr_):
    ImageButton (x_, y_, w_, h_),
    rctr (rctr_)
    {}

void AddSquareBtn::Press() {
    rctr -> AddSquare (100);
}
