#pragma once
#include "BehaviorNode.h"

class L_MoveToNearestCar : public BaseNode<L_MoveToNearestCar>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;
};