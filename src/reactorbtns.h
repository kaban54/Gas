#ifndef REACTORBTNS_H
#define REACTORBTNS_H

#include "gas.h"
#include "buttons.h"

class ReactorBtn : public ImageButton {
    public:
    Reactor* rctr;

    explicit ReactorBtn (double x_, double y_, double w_, double h_, Reactor* rctr_);
};

class AddCircleBtn : public ReactorBtn {
    public:
    explicit AddCircleBtn (double x_, double y_, double w_, double h_, Reactor* rctr_);

    virtual void Press() override;

    virtual void Release() override {}
};


class AddSquareBtn : public ReactorBtn {
    public:
    explicit AddSquareBtn (double x_, double y_, double w_, double h_, Reactor* rctr_);

    virtual void Press() override;

    virtual void Release() override {}
};


class HeatWallsBtn : public ReactorBtn {
    double temp_change;

    public:
    explicit HeatWallsBtn (double x_, double y_, double w_, double h_, Reactor* rctr_, double temp_change_);

    virtual void Press() override {}

    virtual void Release() override;
};


class AcceleratePistonBtn : public ReactorBtn {
    double vel_change;

    public:
    explicit AcceleratePistonBtn (double x_, double y_, double w_, double h_, Reactor* rctr_, double vel_change_);

    virtual void Press() override {}

    virtual void Release() override;
};

#endif