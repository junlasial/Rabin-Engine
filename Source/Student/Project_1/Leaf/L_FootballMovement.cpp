#include <pch.h>
#include "L_FootballMovement.h"


void L_FootballMovement::on_enter()
{
 
    
    Vec3 topLeft = { 34,0,16 };
    Vec3 topRight = { 34,0, 83 };
    Vec3 botLeft = { 16, 0 ,16 };
    Vec3 botRight = { 16,0,83 };





	BehaviorNode::on_leaf_enter();
}

void L_FootballMovement::on_update(float dt)
{
  


    const auto result = agent->move_toward_point(targetPoint, dt);

    if (result == true)
    {
        on_success();
    }


    display_leaf_text();
}
