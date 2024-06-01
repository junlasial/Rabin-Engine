#include <pch.h>
#include "L_CreateBaby.h"
#include "Projects/ProjectOne.h"

void L_CreateBaby::on_enter()
{
 
    
 
    bool reproduce = RNG::coin_toss();

    if (reproduce) {
        // Create an agent (using the default "Agent::AgentModel::Man" model)
        auto baby = agents->create_behavior_agent("baby", BehaviorTreeTypes::Bird,Agent::AgentModel::Bird);


        baby->set_scaling(Vec3{ 0.01,0.01,0.01 });
        baby->set_position(Vec3{ 60, 0, 40 });
        BehaviorNode::on_leaf_enter();

    }

    else {


        on_failure();
    }
 

    
     


    





}

void L_CreateBaby::on_update(float dt)
{
  


 
        on_success();
    


    display_leaf_text();
}
