#include <pch.h>
#include "L_StopCar.h"



void L_StopCar::on_enter()
{
      
    agent->set_movement_speed(0);

	BehaviorNode::on_leaf_enter();
}

void L_StopCar::on_update(float dt)
{
  
 
        on_success();
    

    display_leaf_text();
}
