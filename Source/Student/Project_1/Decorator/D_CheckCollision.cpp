#include <pch.h>
#include "D_CheckCollision.h"



void D_CheckCollision::on_enter()
{


    // get a list of all current agents
    const auto& allAgents = agents->get_all_agents();
    // and our agent's position
    const auto& currPos = agent->get_position();
    auto& bb = agent->get_blackboard();

    for (const auto& a : allAgents)
    {
        // make sure it's not our agent
        if (a != agent)
        {


            if (agent->getAgentModel() == Agent::AgentModel::Car) {
            
                if (a->getAgentModel() == Agent::AgentModel::Car) {

                    auto id = a->get_id();



                    if (id == 0) {

                        const auto& agentPos = a->get_position();
                        const float distance = Vec3::Distance(currPos, agentPos);

                        if (distance <= 25)
                        {
                            bb.set_value("collide", bool(true));
                            collision = bb.get_value<bool>("collide");
                            std::cout << "Collision" << std::endl;
                            BehaviorNode::on_enter();
                        }

                        else {
                            bb.set_value("collide", bool(false));
                            collision = bb.get_value<bool>("collide");
                        }
                    }






                    
                }
            
            }


            if (agent->getAgentModel() == Agent::AgentModel::Ball) {

                if (a->getAgentModel() == Agent::AgentModel::Man) {


                    auto id = a->get_id();

                    if (id == 0) {
                        const auto& agentPos = a->get_position();
                        const float distance = Vec3::Distance(currPos, agentPos);


                        if (distance <= 10)
                        {
                            bb.set_value("ballcollideA", bool(true));
                           
                            BehaviorNode::on_enter();

                        }

                        else {
                            bb.set_value("ballcollideA", bool(false));
                          
                        }
                    }



                    if (id == 1) {
                        const auto& agentPos = a->get_position();
                        const float distance = Vec3::Distance(currPos, agentPos);


                        if (distance <= 8)
                        {
                            bb.set_value("ballcollideB", bool(true));
                           
                            std::cout << "Ball collide la sial" << std::endl;
                            BehaviorNode::on_enter();

                        }

                        else {
                            bb.set_value("ballcollideB", bool(false));
                           
                        }
                    }


                }
            }





        }
    }



}

void D_CheckCollision::on_update(float dt)
{





    // Check if enteredCar is true
    if (collision)
    {

       
        // If entered a car1, proceed to the child node
        BehaviorNode* child = children.front();
        child->tick(dt);
        // assume same status as child
        set_status(child->get_status());
        set_result(child->get_result());
    }
    else
    {
        // If not entered a car, do nothing or perform other actions as needed
        // For example, you might want to log some message indicating that entering a car condition wasn't met.
     
        BehaviorNode* child = children.front();
        child->tick(dt);
        // assume same status as child
        set_status(child->get_status());
        set_result(child->get_result());
    
    }


  

    // Optionally, you can still display leaf text regardless of entering a car or not
    display_leaf_text();
}
