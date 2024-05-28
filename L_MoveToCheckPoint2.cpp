#include <pch.h>
#include "L_MoveToCheckPoint2.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToCheckPoint2::on_enter()
{
    // write the world position into the blackboard
    
       // grab the target position from the blackboard
    const auto& bb = agent->get_blackboard();
    targetPoint = bb.get_value<Vec3>("CheckPoint2");



    BehaviorNode::on_leaf_enter();
    
}

void L_MoveToCheckPoint2::on_update(float dt)
{
    const auto result = agent->move_toward_point(targetPoint, dt);

    // grab the target position from the blackboard


    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}

void L_MoveToCheckPoint2::on_exit() {

    auto& bb = agent->get_blackboard();
    bb.set_value("Current", Vec3{ 2,0,0 });


}

