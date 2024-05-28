#include <pch.h>
#include "L_MoveToNearestCar.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToNearestCar::on_enter()
{
    // set animation, speed, etc

   // find the agent that is the furthest from this one
    float longestDistance = std::numeric_limits<float>().max();
    Vec3 furthestPoint;
    bool targetFound = false;

    // get a list of all current agents
    const auto& allAgents = agents->get_all_agents();

    // and our agent's position
    const auto& currPos = agent->get_position();

    for (const auto& a : allAgents)
    {
        // make sure it's not our agent
        if (a != agent)
        {
            if (a->getAgentModel() == Agent::AgentModel::Car) {


                const auto& agentPos = a->get_position();
                const float distance = Vec3::Distance(currPos, agentPos);

                if (distance < longestDistance)
                {
                    longestDistance = distance;
                    furthestPoint = agentPos;
                    targetFound = true;
                }
            }

           
        }
    }

    if (targetFound == true)
    {
        targetPoint = furthestPoint;
        BehaviorNode::on_leaf_enter();
    }
    else // couldn't find a vixble agent
    {
        on_failure();
    }
}

void L_MoveToNearestCar::on_update(float dt)
{
    const auto result = agent->move_toward_point(targetPoint, dt);


    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}

