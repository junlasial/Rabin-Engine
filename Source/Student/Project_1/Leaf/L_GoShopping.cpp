#include <pch.h>
#include "L_GoShopping.h"
#include "Agent/BehaviorAgent.h"

void L_GoShopping::on_enter()
{
    // write the world position into the blackboard

    const auto& bb = agent->get_blackboard();
    targetPoint = bb.get_value<Vec3>("ShoppingMall");



    BehaviorNode::on_leaf_enter();

}

void L_GoShopping::on_update(float dt)
{
    static bool waitingForSuccess = false;
    static float waitTime = 0.0f;
     auto& bb = agent->get_blackboard();

    if (bb.get_value<bool>("GoMall") == true) {

      
        if (!waitingForSuccess)
        {


            const auto result = agent->move_toward_point(targetPoint, dt);

            if (result == true)
            {
               
                // Start the waiting period
                waitingForSuccess = true;
                waitTime = 0.0f; // Reset the wait time
            }
        }
        else
        {
            // Accumulate the wait time
            waitTime += dt;
            if (waitTime >= 15.0f)
            {
           
              
                // Wait time has elapsed
                on_success();
                waitingForSuccess = false; // Reset the state for the next target
            }
        }

    }


    else {

        on_failure();
    }
   

    display_leaf_text();
}


