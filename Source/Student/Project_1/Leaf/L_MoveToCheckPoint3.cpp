#include <pch.h>
#include "L_MoveToCheckPoint3.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToCheckPoint3::on_enter()
{
    targetPoint = { 5,0,10 };
    BehaviorNode::on_leaf_enter();
}

void L_MoveToCheckPoint3::on_update(float dt)
{
    const auto result = agent->move_toward_point(targetPoint, dt);


    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}

