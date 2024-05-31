#include <pch.h>
#include "L_CheckMouseClick.h"
#include "../Agent/BehaviorAgent.h"

void L_CheckMouseClick::on_update(float dt)
{
    const auto leftMouseState = InputHandler::get_current_state(MouseButtons::LEFT);

    if (leftMouseState == InputHandler::InputState::PRESSED)
    {
        // grab the current mouse pos
        const auto &mousePos = InputHandler::get_mouse_position();

        // we want to know where on the ground the mouse was clicked
        const auto &plane = terrain->get_terrain_plane();

        // find out where on the plane the click happened
        const auto worldPos = renderer->screen_to_world(mousePos.x, mousePos.y, plane);

        // if the click point was actually on the plane
        if (worldPos.second == true)
        {
            // write the world position into the blackboard
            auto &bb = agent->get_blackboard();
            bb.set_value("Click Position", worldPos.first);
            
  /*          Vec3 mouse= bb.get_value<Vec3>("Click Position");

            std::cout << "X Position: "<< mouse.x << std::endl;
            std::cout << "Z Position: " << mouse.z << std::endl;*/
            on_success();
        }
        else
        {
            on_failure();
        }
    }
    else
    {
        on_failure();
    }

    display_leaf_text();
}
