#pragma once
#include "BehaviorNode.h"

class L_MoveToCheckPoint6 : public BaseNode<L_MoveToCheckPoint6>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;
    
};
