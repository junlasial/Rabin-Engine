#pragma once
#include "BehaviorNode.h"

class L_GoCar : public BaseNode<L_GoCar>
{


protected:
    float timer;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};