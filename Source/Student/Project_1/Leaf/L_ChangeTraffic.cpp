#include <pch.h>
#include "L_ChangeTraffic.h"

L_ChangeTraffic::L_ChangeTraffic() : timer(0.0f)
{}

void L_ChangeTraffic::on_enter()
{
    // Start with red
    agent->set_color(Vec3{ 1,0,0 });

    // Set initial timer values
    redTimer = 8.0f;
    greenTimer = 5.0f;
    yellowTimer = 1.0f;


    BehaviorNode::on_leaf_enter();
}

void L_ChangeTraffic::on_update(float dt)
{
    // get a list of all current agents
    const auto& allAgents = agents->get_all_agents();
    // and our agent's position
    const auto& currPos = agent->get_position();

    auto& bb = agent->get_blackboard();

    if (redTimer > 0.0f)
    {

        bb.set_value("CurrentLight", Vec3{1,0,0});

        // Red phase
        redTimer -= dt;
        if (redTimer <= 0.0f)
        {
            // Switch to green
            agent->set_color(Vec3{ 0,1,0 });
            greenTimer = 5.0f;
        }
    }
    else if (greenTimer > 0.0f)
    {

        bb.set_value("CurrentLight", Vec3{ 0,1,0 });
        // Green phase
        greenTimer -= dt;
        if (greenTimer <= 0.0f)
        {
            // Switch to yellow
            agent->set_color(Vec3{ 1,1,0 });
            yellowTimer = 1.0f;
        }
    }
    else
    {

        bb.set_value("CurrentLight", Vec3{ 1,1,0 });


        // Yellow phase
        yellowTimer -= dt;
        if (yellowTimer <= 0.0f)
        {
            // Switch back to red and reset timers
            agent->set_color(Vec3{ 1,0,0 });
            redTimer = 8.0f;
        }
    }

    for (const auto& a : allAgents)
    {

        // make sure it's not our agent
        if (a != agent)
        {
            if (a->getAgentModel() == Agent::AgentModel::Car) {
                const auto& agentPos = a->get_position();
                const float distance = Vec3::Distance(currPos, agentPos);
                Vec3 currLight = bb.get_value<Vec3>("CurrentLight");

          

                if (distance <= 10) {


                    if (currLight == Vec3(1,0,0)) {

                        
                        a->set_movement_speed(0);
                      
           
                    }
                    
                    
                  
                    else if (currLight == Vec3(1, 1, 0)) {
                        a->set_movement_speed(30);
                    }

                    else {
                        a->set_movement_speed(25);

                    }


                }

                


            }




        }



    }




    display_leaf_text();
}

