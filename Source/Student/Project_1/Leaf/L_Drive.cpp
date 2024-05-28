#include <pch.h>
#include "L_Drive.h"
#include "Agent/BehaviorAgent.h"

void L_Drive::on_enter()
{


     BehaviorNode::on_leaf_enter();

}

void L_Drive::on_update(float dt)
{
    const auto result = agent->move_toward_point(targetPoint, dt);
    

    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}

