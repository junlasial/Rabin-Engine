#include <pch.h>
#include "L_MoveToCorner.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToCorner::on_enter()
{
  
    
    //targetPoint = RNG::world_position();
    targetPoint = { 100,0,100 };
    //agent->look_at_point(targetPoint);

    BehaviorNode::on_leaf_enter();



}

void L_MoveToCorner::on_update(float dt)
{


    const auto result = agent->move_toward_point(targetPoint, dt);
    // set animation, speed, etc

   
    
    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}
