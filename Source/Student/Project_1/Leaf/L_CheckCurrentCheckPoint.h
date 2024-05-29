#pragma once
#include "BehaviorNode.h"

class L_CheckCurrentCheckPoint : public BaseNode<L_CheckCurrentCheckPoint>
{

private:

    bool invert = false;;

protected:
    virtual void on_update(float dt);
};