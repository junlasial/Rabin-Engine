#include <pch.h>
#include "L_ChangeTraffic.h"

L_ChangeTraffic::L_ChangeTraffic() : timer(0.0f)
{}

void L_ChangeTraffic::on_enter()
{
    // Start with red
    agent->set_color(Vec3{ 1,0,0 });

    // Set initial timer values
    redTimer = 5.0f;
    greenTimer = 10.0f;
    yellowTimer = 1.0f;


    BehaviorNode::on_leaf_enter();
}

void L_ChangeTraffic::on_update(float dt)
{

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
            greenTimer = 10.0f;
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
            redTimer = 5.0f;
        }
    }

    display_leaf_text();
}

