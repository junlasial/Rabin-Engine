#include <pch.h>
#include "L_MoveToCheckPoint3.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToCheckPoint3::on_enter()
{
    const auto& bb = agent->get_blackboard();
    targetPoint = bb.get_value<Vec3>("CheckPoint3");


    BehaviorNode::on_leaf_enter();
    // grab the target position from the blackboard

   
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

