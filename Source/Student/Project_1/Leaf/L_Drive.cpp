#include <pch.h>
#include "L_Drive.h"
#include "Agent/BehaviorAgent.h"
#include <random> // Include the <random> library for random number generation


void L_Drive::on_enter()
{

        const auto& currPos = agent->get_position();

        CurrentPosition = currPos;
        const auto & bb = agent->get_blackboard();
        Checkpoint_1 = bb.get_value<Vec3>("CheckPoint1");
        Checkpoint_2 = bb.get_value<Vec3>("CheckPoint2");
        Checkpoint_3 = bb.get_value<Vec3>("CheckPoint3");
        Checkpoint_4 = bb.get_value<Vec3>("CheckPoint4");
        Checkpoint_5 = bb.get_value<Vec3>("CheckPoint5");
        Checkpoint_6 = bb.get_value<Vec3>("CheckPoint6");


        float thresholdSquared = 0.02f; // Threshold squared, adjust as needed
        auto distanceSquared = [](const Vec3& v1, const Vec3& v2) {
            float dx = v1.x - v2.x;
            float dy = v1.y - v2.y;
            float dz = v1.z - v2.z;
            return dx * dx + dy * dy + dz * dz;
            };


        // Randomly choose between going to checkpoint 2 or 6
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, 1); // 0 for Checkpoint_2, 1 for Checkpoint_6
        int choice = distrib(gen);



        if (distanceSquared(CurrentPosition, Checkpoint_1) < thresholdSquared) {

            targetPoint = Checkpoint_2;
        }
        else if (distanceSquared(CurrentPosition, Checkpoint_2) < thresholdSquared) {
            targetPoint = Checkpoint_5;
        }
        else if (distanceSquared(CurrentPosition, Checkpoint_3) < thresholdSquared) {
            targetPoint = Checkpoint_4;
        }
        else if (distanceSquared(CurrentPosition, Checkpoint_4) < thresholdSquared) {
            targetPoint = Checkpoint_6;
        }
        else if (distanceSquared(CurrentPosition, Checkpoint_5) < thresholdSquared) {
            if (choice == 0) {
                targetPoint = Checkpoint_3;
            }

            else {
                targetPoint = Checkpoint_6;
              
            }
        }
        else if (distanceSquared(CurrentPosition, Checkpoint_6) < thresholdSquared) {
            if (choice == 0) {
                targetPoint = Checkpoint_5;
            }

            else {
                targetPoint = Checkpoint_1;
            }
        }



     BehaviorNode::on_leaf_enter();

}

void L_Drive::on_update(float dt)
{
    const auto result = agent->move_toward_point(targetPoint, dt);
    

    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}

