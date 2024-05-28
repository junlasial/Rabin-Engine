#pragma once
#include "BehaviorNode.h"

class L_Drive : public BaseNode<L_Drive>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;
    Vec3 Checkpoint_1;
    Vec3 Checkpoint_2;
    Vec3 Checkpoint_3;
    Vec3 Checkpoint_4;
    Vec3 Checkpoint_5;
    Vec3 Checkpoint_6;
};