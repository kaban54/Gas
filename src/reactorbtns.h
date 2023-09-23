#ifndef REACTORBTNS_H
#define REACTORBTNS_H

#include "gas.h"
#include "buttons.h"

class AddCircleBtn : public ImageButton {
    Reactor* rctr;

    public:
    explicit AddCircleBtn (double x_, double y_, double w_, double h_, Reactor* rctr_);

    virtual void Press() override;

    virtual void Release() override {}
};

class AddSquareBtn : public ImageButton {
    Reactor* rctr;

    public:
    explicit AddSquareBtn (double x_, double y_, double w_, double h_, Reactor* rctr_);

    virtual void Press() override;

    virtual void Release() override {}
};

#endif