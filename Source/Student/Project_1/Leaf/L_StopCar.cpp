#include <pch.h>
#include "L_StopCar.h"



#include <chrono>
#include <thread>

void L_StopCar::on_enter()
{
    auto& bb = agent->get_blackboard();
    if (bb.get_value<bool>("collide") == true)
    {
        // Slow down to speed 5
        agent->set_movement_speed(2);

        // Create a separate thread to handle the delayed action
        std::thread([this]() {
            // Sleep for 5 seconds
            std::this_thread::sleep_for(std::chrono::seconds(7));

            // After 5 seconds, speed up to 15
            agent->set_movement_speed(15);
            }).detach(); // Detach the thread to let it run independently
    }

    BehaviorNode::on_leaf_enter();
}

void L_StopCar::on_update(float dt)
{
  


 
        on_success();


    

    display_leaf_text();
}
