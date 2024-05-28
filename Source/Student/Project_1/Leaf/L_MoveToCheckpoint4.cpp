#include <pch.h>
#include "L_MoveToCheckPoint4.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToCheckPoint4::on_enter()
{
    // write the world position into the blackboard

    const auto& bb = agent->get_blackboard();
    targetPoint = bb.get_value<Vec3>("CheckPoint4");

    BehaviorNode::on_leaf_enter();

}

void L_MoveToCheckPoint4::on_update(float dt)
{
    const auto result = agent->move_toward_point(targetPoint, dt);


    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}

