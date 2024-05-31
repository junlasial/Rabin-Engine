#include <pch.h>
#include "L_BallMove.h"
#include "Agent/BehaviorAgent.h"
#include <random>
void L_BallMove::on_enter()
{
    // get a list of all current agents
    const auto& allAgents = agents->get_all_agents();
    // and our agent's position
    const auto& currPos = agent->get_position();
    auto& bb = agent->get_blackboard();


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 5.0);

  
    for (const auto& a : allAgents)
    {
        // make sure it's not our agent
        if (a != agent)
        {
            if (a->getAgentModel() == Agent::AgentModel::Man) {



                auto id = a->get_id();

                if (id == 0) {


                    if (bb.get_value<bool>("ballcollideB")) {


                        targetPoint = a->get_position();
                        finalPoint = targetPoint;
                        finalPoint.y = targetPoint.y + static_cast<int>(dis(gen));

                    }


                }


                if (id == 1) {

                    if (bb.get_value<bool>("ballcollideA")) {

                        targetPoint = a->get_position();
                        finalPoint = targetPoint;
                        finalPoint.y = targetPoint.y + static_cast<int>(dis(gen));


                    }
                }
      


         


            }
            


        }


    }



    BehaviorNode::on_leaf_enter();

}

void L_BallMove::on_update(float dt)
{
    const auto result = agent->move_toward_point(finalPoint, dt);

    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}

