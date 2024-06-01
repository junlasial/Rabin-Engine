#include <pch.h>
#include "L_GoHouse.h"
#include "Agent/BehaviorAgent.h"
#include <random> // For random number generation
#include <array>  // For std::array

void L_GoHouse::on_enter()
{
    // Use a non-deterministic random number generator for seeding
    std::random_device rd;

    // Use the Mersenne Twister engine
    std::mt19937 generator(rd());

    // Define a uniform distribution to generate numbers between 0 and 2
    std::uniform_int_distribution<int> distribution(0, 2);

    // Array of possible target keys
    std::array<const char*, 3> targets = { "ShoppingMall", "ShoppingMall1", "ShoppingMall2" };

    // Randomly select one of the targets
    int randomIndex = distribution(generator);
    const char* selectedTarget = targets[randomIndex];

    // Write the selected target's world position into the blackboard
    const auto& bb = agent->get_blackboard();
    targetPoint = bb.get_value<Vec3>(selectedTarget);

    BehaviorNode::on_leaf_enter();
}


void L_GoHouse::on_update(float dt)
{


    static bool waitingForSuccess = false;
    static float waitTime = 0.0f;
    auto& bb = agent->get_blackboard();

  

        if (!waitingForSuccess)
        {


            const auto result = agent->move_toward_point(targetPoint, dt);

            if (result == true)
            {
                agent->set_scaling(Vec3{ 0,0,0 });
                // Start the waiting period
                waitingForSuccess = true;
                waitTime = 0.0f; // Reset the wait time
            }
        }
        else
        {
            // Accumulate the wait time
            waitTime += dt;
            if (waitTime >= 5.0f)
            {
                agent->set_scaling(Vec3{ 1.5,1.5,1.5 });

                // Wait time has elapsed
                on_success();
                waitingForSuccess = false; // Reset the state for the next target
            }
        }

    


  

    display_leaf_text();
}

