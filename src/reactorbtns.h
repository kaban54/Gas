#ifndef REACTORBTNS_H
#define REACTORBTNS_H

#include "reactor.h"
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

    virtual void Pressed (double dt) override {}

    virtual void Release() override {}
};


class AddSquareBtn : public ReactorBtn {
    public:
    explicit AddSquareBtn (double x_, double y_, double w_, double h_, Reactor* rctr_);

    virtual void Press() override;

    virtual void Pressed (double dt) override {}

    virtual void Release() override {}
};


class HeatWallsBtn : public ReactorBtn {
    double temp_change_v;

    public:
    explicit HeatWallsBtn (double x_, double y_, double w_, double h_, Reactor* rctr_, double temp_change_v_);

    virtual void Press() override {}

    virtual void Pressed (double dt) override;

    virtual void Release() override {};
};


class MovePistonBtn : public ReactorBtn {
    double vel;

    public:
    explicit MovePistonBtn (double x_, double y_, double w_, double h_, Reactor* rctr_, double vel_);

    virtual void Press() override;

    virtual void Pressed (double dt) override;

    virtual void Release() override;
};

#endif