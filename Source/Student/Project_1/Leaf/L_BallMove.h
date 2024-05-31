#pragma once
#include "BehaviorNode.h"

class L_BallMove : public BaseNode<L_BallMove>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;
    Vec3 finalPoint;
  
};