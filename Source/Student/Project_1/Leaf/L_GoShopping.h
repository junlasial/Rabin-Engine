#pragma once
#include "BehaviorNode.h"

class L_GoShopping : public BaseNode<L_GoShopping>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;
    
};
