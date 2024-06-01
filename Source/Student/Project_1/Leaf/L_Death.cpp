#include <pch.h>
#include "L_Death.h"


void L_Death::on_enter()
{
 
    bool death = RNG::coin_toss();

    if (death) {

        bool secondchance = RNG::coin_toss();

        if (secondchance) {
            agents->destroy_agent(agent);
            std::cout << "Agent Died lol" << std::endl;

        }

    }

 

    BehaviorNode::on_leaf_enter();


}

void L_Death::on_update(float dt)
{
  



        on_success();
    


    display_leaf_text();
}
