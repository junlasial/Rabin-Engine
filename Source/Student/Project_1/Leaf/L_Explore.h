#pragma once
#include "BehaviorNode.h"

class L_Explore : public BaseNode<L_Explore>
{


private:


    Vec3 targetPoint;

protected:


    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};