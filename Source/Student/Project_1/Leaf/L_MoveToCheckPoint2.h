#pragma once
#include "BehaviorNode.h"

class L_MoveToCheckPoint2 : public BaseNode<L_MoveToCheckPoint2>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;
};