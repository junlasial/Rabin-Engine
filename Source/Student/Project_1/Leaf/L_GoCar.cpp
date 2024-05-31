#include <pch.h>
#include "L_GoCar.h"



void L_GoCar::on_enter()
{
      
    auto& bb = agent->get_blackboard();
    if (bb.get_value<bool>("collide") == false)
    {
        agent->set_movement_speed(15);

    }

    BehaviorNode::on_leaf_enter();
}

void L_GoCar::on_update(float dt)
{
  
    auto& bb = agent->get_blackboard();
    if (bb.get_value<bool>("collide") == false)
    {
        on_success();

    }

    else {

        on_failure();
    }
       
    

    display_leaf_text();
}
