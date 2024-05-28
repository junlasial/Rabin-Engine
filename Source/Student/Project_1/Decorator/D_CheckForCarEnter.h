#pragma once
#pragma once
#include "BehaviorNode.h"

class D_CheckForCarEnter : public BaseNode<D_CheckForCarEnter>
{

public:

    bool enteredCar = false;


protected:

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
}; 