#pragma once
#include "BehaviorNode.h"

class L_CheckCurrentCheckPoint : public BaseNode<L_CheckCurrentCheckPoint>
{
protected:
    virtual void on_update(float dt);
};