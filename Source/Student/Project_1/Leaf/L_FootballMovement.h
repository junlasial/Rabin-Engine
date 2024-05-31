#pragma once
#include "BehaviorNode.h"

class L_FootballMovement : public BaseNode<L_FootballMovement>
{


private:


    Vec3 targetPoint;

protected:


    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};