#pragma once
#pragma once
#include "BehaviorNode.h"

class D_CheckReachMall : public BaseNode<D_CheckReachMall>
{

public:

    bool reachMall = false;


protected:

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
}; 