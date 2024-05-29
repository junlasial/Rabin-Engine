#include <pch.h>
#include "D_CheckReachMall.h"



void D_CheckReachMall::on_enter()
{


    // get a list of all current agents
    const auto& allAgents = agents->get_all_agents();
    // and our agent's position
    const auto& currPos = agent->get_position();

    for (const auto& a : allAgents)
    {
        // make sure it's not our agent
        if (a != agent)
        {
            if (a->getAgentModel() == Agent::AgentModel::Tree) {
                const auto& agentPos = a->get_position();
                const float distance = Vec3::Distance(currPos, agentPos);

                if (distance >= 15)
                {
                    reachMall = false;
                  //  std::cout << "Have not reached" << std::endl;
                    BehaviorNode::on_enter();
                }

                else {

                    reachMall = true;

                }
            }
            
            if (a->getAgentModel() == Agent::AgentModel::Man) {
                const auto& agentPos = a->get_position();
                const float distance = Vec3::Distance(currPos, agentPos);

                if (distance >= 15)
                {
                    reachMall = false;
                    //std::cout << "Have not reached" << std::endl;
                    BehaviorNode::on_enter();
                }

                else {

                    reachMall = true;
                }
            }

          

        }
    }




}

void D_CheckReachMall::on_update(float dt)
{
    // Check if enteredCar is true
    if (reachMall)
    {
        // If entered a car, proceed to the child node
        BehaviorNode* child = children.front();
        child->tick(dt);
        // assume same status as child
        set_status(child->get_status());
        set_result(child->get_result());
    }
    else
    {

        on_failure();
        // If not entered a car, do nothing or perform other actions as needed
        // For example, you might want to log some message indicating that entering a car condition wasn't met.
        //std::cout << "Not reached mall" << std::endl;
    }

    // Optionally, you can still display leaf text regardless of entering a car or not
    display_leaf_text();
}
