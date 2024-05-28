#pragma once
#include "BehaviorNode.h"

class L_ChangeTraffic : public BaseNode<L_ChangeTraffic>
{

public:

    L_ChangeTraffic();

protected:
    float timer;
    float redTimer;
    float greenTimer;
    float yellowTimer;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};