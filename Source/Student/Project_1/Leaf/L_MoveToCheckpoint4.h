#pragma once
#include "BehaviorNode.h"

class L_MoveToCheckPoint4 : public BaseNode<L_MoveToCheckPoint4>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;
    
};
