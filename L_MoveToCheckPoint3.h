#pragma once
#include "BehaviorNode.h"

class L_MoveToCheckPoint3 : public BaseNode<L_MoveToCheckPoint3>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;
   
};