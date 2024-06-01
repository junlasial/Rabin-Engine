#pragma once
#include "BehaviorNode.h"

class L_GoHouse : public BaseNode<L_GoHouse>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;
    
};
