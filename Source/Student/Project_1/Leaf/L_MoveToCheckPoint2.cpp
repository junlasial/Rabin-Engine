#include <pch.h>
#include "L_MoveToCheckPoint2.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToCheckPoint2::on_enter()
{
    targetPoint = { 80,0,80 };
    BehaviorNode::on_leaf_enter();
}

void L_MoveToCheckPoint2::on_update(float dt)
{
    const auto result = agent->move_toward_point(targetPoint, dt);


    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}

