#include <pch.h>
#include "L_Explore.h"


void L_Explore::on_enter()
{
 
    
    //Vec3 topLeft = { 34,0,16 };
    //Vec3 topRight = { 34,0, 83 };
    //Vec3 botLeft = { 16, 0 ,16 };
    //Vec3 botRight = { 16,0,83 };




        //Pos allowed for footballer1
        targetPoint = RNG::explore_position();


        if (agent->getAgentModel() == Agent::AgentModel::Bird) {
            agent->set_scaling(Vec3{ 0.01,0.01,0.01 });
        }

        else {
            agent->set_scaling(Vec3{ 1.5,1.5,1.5 });
        }
     

    
        BehaviorNode::on_leaf_enter();


    


    //agent->look_at_point(targetPoint);



}

void L_Explore::on_update(float dt)
{
  


    const auto result = agent->move_toward_point(targetPoint, dt);

    if (result == true)
    {
        on_success();
    }


    display_leaf_text();
}
