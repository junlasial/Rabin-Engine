#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_FlyToRandomPosition : public BaseNode<L_FlyToRandomPosition>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;
};