#include <pch.h>
#include "L_FlyToRandomPosition.h"
#include "Agent/BehaviorAgent.h"

void L_FlyToRandomPosition::on_enter()
{
    // set animation, speed, etc

    targetPoint = RNG::bird_position();
    //agent->look_at_point(targetPoint);

	BehaviorNode::on_leaf_enter();
}

void L_FlyToRandomPosition::on_update(float dt)
{
    const auto result = agent->move_toward_point(targetPoint, dt);

    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}
