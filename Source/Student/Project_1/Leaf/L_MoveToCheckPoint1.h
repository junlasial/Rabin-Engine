#pragma once
#include "BehaviorNode.h"

class L_MoveToCheckPoint1 : public BaseNode<L_MoveToCheckPoint1>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;
  
};
