#include <pch.h>
#include "L_ChangeTraffic.h"
#include <random>
#include "Agent/CameraAgent.h"

// Constructor
L_ChangeTraffic::L_ChangeTraffic() : timer(0.0f), redTimer(0.0f), greenTimer(0.0f), yellowTimer(0.0f)
{}

// on_enter method
void L_ChangeTraffic::on_enter()
{
    // Create a random engine and distribution
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(2.0, 5.0);

    // Start with red
    agent->set_color(Vec3{ 1,0,0 });

    // Set initial timer values with random offsets, cast to float
    redTimer = 3.0f + static_cast<float>(dis(gen));
    greenTimer = 3.0f + static_cast<float>(dis(gen));
    yellowTimer = 1.0f + static_cast<float>(dis(gen));

    BehaviorNode::on_leaf_enter();
}

// on_update method
void L_ChangeTraffic::on_update(float dt)




{
    // Get a list of all current agents
    const auto& allAgents = agents->get_all_agents();
    // Our agent's position
    const auto& currPos = agent->get_position();

    // Blackboard
    auto& bb = agent->get_blackboard();

    if (redTimer > 0.0f)
    {
        bb.set_value("CurrentLight", Vec3{ 1,0,0 });

        // Red phase
        redTimer -= dt;
        if (redTimer <= 0.0f)
        {
            // Switch to green
            agent->set_color(Vec3{ 0,1,0 });
            greenTimer = 3.0f;
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
            redTimer = 3.0f;
        }
    }

    for (const auto& a : allAgents)
    {
        // Make sure it's not our agent
        if (a != agent)
        {
            if (a->getAgentModel() == Agent::AgentModel::Car) {

               auto id = a->get_id();

            
                const auto& agentPos = a->get_position();
                const float distance = Vec3::Distance(currPos, agentPos);
                Vec3 currLight = bb.get_value<Vec3>("CurrentLight");

                if (distance <= 10) {
                    if (currLight == Vec3(1, 0, 0)) { // red
                        a->set_movement_speed(0.1);
                    }
                    else if (currLight == Vec3(1, 1, 0)) { // yellow
                        if (id == 0) {
                            a->set_movement_speed(12); //green
                        }
                        if (id == 1) {
                            a->set_movement_speed(18); //green

                        }
                    }
                    else {
                        if (id == 0) {
                            a->set_movement_speed(10); //green
                        }
                        if (id == 1) {
                            a->set_movement_speed(17); //green

                        }
                       
                    }
                }
            }
        }
    }

    display_leaf_text();
}
