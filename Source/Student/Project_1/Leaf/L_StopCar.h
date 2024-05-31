#pragma once
#include "BehaviorNode.h"

class L_StopCar : public BaseNode<L_StopCar>
{


protected:
    float timer;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};