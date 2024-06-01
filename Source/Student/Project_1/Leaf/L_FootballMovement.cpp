#include <pch.h>
#include "L_FootballMovement.h"


void L_FootballMovement::on_enter()
{
 
    
    //Vec3 topLeft = { 34,0,16 };
    //Vec3 topRight = { 34,0, 83 };
    //Vec3 botLeft = { 16, 0 ,16 };
    //Vec3 botRight = { 16,0,83 };



    if (agent->get_id() == 0) {


        //Pos allowed for footballer1
        targetPoint = RNG::footballer_position1();

        BehaviorNode::on_leaf_enter();

    }

    if (agent->get_id() == 1) {
        //Pos allowed for footballer2
        targetPoint = RNG::footballer_position2();

        BehaviorNode::on_leaf_enter();

    }


    


    //agent->look_at_point(targetPoint);



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
