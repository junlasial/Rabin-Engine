#pragma once
#include "BehaviorNode.h"

class L_CreateBaby : public BaseNode<L_CreateBaby>
{


private:


    Vec3 targetPoint;

protected:


    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};