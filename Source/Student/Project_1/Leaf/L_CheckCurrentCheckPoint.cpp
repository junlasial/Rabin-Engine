#include <pch.h>
#include "L_CheckCurrentCheckPoint.h"
#include "../Agent/BehaviorAgent.h"

void L_CheckCurrentCheckPoint::on_update(float dt)
{
    
    // grab the current mouse pos
    const auto& currPos = agent->get_position();

    auto& bb = agent->get_blackboard();
    bb.set_value("CheckPoint1", Vec3{ 5,0,90 });
    bb.set_value("CheckPoint2", Vec3{ 5,0,10 });
    bb.set_value("CheckPoint3", Vec3{ 90,0,10 });
    bb.set_value("CheckPoint4", Vec3{ 90,0,90 });
    bb.set_value("CheckPoint5", Vec3{ 45,0,10 });
    bb.set_value("CheckPoint6", Vec3{ 45,0,90 });
    bb.set_value("ShoppingMall", Vec3{ 75, 0, 73 });

    const float range = 9.0f; // Define the range within which we consider the positions to be "equal"

    const auto& checkPoint3 = bb.get_value<Vec3>("CheckPoint3");
    const float distance = Vec3::Distance(currPos, checkPoint3);

    if (distance <= range) {

        invert = !invert;
     
        bb.set_value("GoMall", invert);


    }




        on_success();
   
  
    display_leaf_text();
}
