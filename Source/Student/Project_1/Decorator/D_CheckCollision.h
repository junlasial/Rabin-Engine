#pragma once
#pragma once
#include "BehaviorNode.h"

class D_CheckCollision : public BaseNode<D_CheckCollision>
{

public:

    bool collision;


protected:

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
}; 