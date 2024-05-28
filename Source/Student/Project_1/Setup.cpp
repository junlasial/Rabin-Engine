#include <pch.h>
#include "Projects/ProjectOne.h"
#include "Agent/CameraAgent.h"

void ProjectOne::setup()
{


    
    // Create an agent (using the default "Agent::AgentModel::Man" model)
    auto trafficlight = agents->create_behavior_agent("TrafficLight", BehaviorTreeTypes::TrafficLight);

    // You can change properties here or at runtime from a behavior tree leaf node
    // Look in Agent.h for all of the setters, like these:
    trafficlight->set_color(Vec3(0, 1, 0));
    trafficlight->set_scaling(Vec3(1,3,3));
    trafficlight->set_position(Vec3(38, 0, 15));
    trafficlight->set_yaw(PI / 2);


     // Create an agent (using the default "Agent::AgentModel::Man" model)
     auto man1 = agents->create_behavior_agent("Driver", BehaviorTreeTypes::MoveToNearestCar);

     // You can change properties here or at runtime from a behavior tree leaf node
     // Look in Agent.h for all of the setters, like these:
     man1->set_color(Vec3(1, 1, 1));
     man1->set_scaling(Vec3(1, 1, 1));
     man1->set_position(Vec3(30, 0, 60));
     
    // Create an agent with a different 3D model:
    // 1. (optional) Add a new 3D model to the framework other than the ones provided:
    //    A. Find a ".sdkmesh" model or use https://github.com/walbourn/contentexporter
    //       to convert fbx files (many end up corrupted in this process, so good luck!)
    //    B. Add a new AgentModel enum for your model in Agent.h (like the existing Man or Tree).
    // 2. Register the new model with the engine, so it associates the file path with the enum
    //    A. Here we are registering all of the extra models that already come in the package.
    Agent::add_model("Assets\\tree.sdkmesh", Agent::AgentModel::Tree);
    Agent::add_model("Assets\\car.sdkmesh", Agent::AgentModel::Car);
    Agent::add_model("Assets\\bird.sdkmesh", Agent::AgentModel::Bird);
    Agent::add_model("Assets\\ball.sdkmesh", Agent::AgentModel::Ball);
    Agent::add_model("Assets\\hut.sdkmesh", Agent::AgentModel::Hut);


    // 3. Create the agent, giving it the correct AgentModel type.
    //auto car1 = agents->create_behavior_agent("Player Car", BehaviorTreeTypes::carmove, Agent::AgentModel::Car);
    //// 4. (optional) You can also set the pitch of the model, if you want it to be rotated differently
  
    //// 5. (optional) Set other aspects to make it start out correctly
    //car1->set_scaling(Vec3(1, 1, 1));
    //car1->set_color(Vec3(1, 0, 0));   // Set the tree to green
    //car1->set_position(Vec3(30, 0, 80));


    auto car2 = agents->create_behavior_agent("Mercedes", BehaviorTreeTypes::gtestubg, Agent::AgentModel::Car);
    // 5. (optional) Set other aspects to make it start out correctly
    car2->set_scaling(Vec3(1, 1, 1));
    car2->set_color(Vec3(0, 0, 1)); // Set the tree to green
    car2->set_position(Vec3(5, 0, 90));
    
    


    //auto hut = agents->create_behavior_agent("ShoppingMall", BehaviorTreeTypes::Idle, Agent::AgentModel::Hut);
    //// 5. (optional) Set other aspects to make it start out correctly
    //hut->set_scaling(Vec3(0.8, 0.8, 0.8));
    //hut->set_color(Vec3(0.5, 0.5, 0.5));   // Set the tree to green
    //hut->set_position(Vec3(80, 0, 0));



  
    // You can technically load any map you want, even create your own map file,
    // but behavior agents won't actually avoid walls or anything special, unless you code
    // that yourself (that's the realm of project 2)
    terrain->goto_map(0);

    // You can also enable the pathing layer and set grid square colors as you see fit.
    // Works best with map 0, the completely blank map
    terrain->pathLayer.set_enabled(true);
  

    //for (int row = 0; row <= 19; ++row) {
    //    for (int col = 0; col <= 19; ++col) {
    //        // If the sum of row and column indices is even, set color to Black,
    //        // otherwise, set color to White
    //        Color color = ((row + col) % 2 == 0) ? Colors::Black : Colors::White;

    //        // Set the color for the current cell
    //        terrain->pathLayer.set_value(row, col, color);
    //    }
    //}



    for (int i = 0; i < 20;  i++) {
        for (int j = 0; j < 3; j++) {
            terrain->pathLayer.set_value(j, i, Colors::Black);
        }
    }


    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 3; j++) {
            terrain->pathLayer.set_value(i, j, Colors::Black);
        }
    }


    // Mirrored loop for the first code block
    for (int i = 19; i >= 0; i--) {
        for (int j = 19; j > 16; j--) {
            terrain->pathLayer.set_value(j,i, Colors::Black);
        }
    }

    // Mirrored loop for the first code block
    for (int i = 19; i >= 0; i--) {
        for (int j = 19; j > 16; j--) {
            terrain->pathLayer.set_value(i, j, Colors::Black);
        }
    }

    for (int i = 0; i < 20; i++) {

        for (int j = 8; j < 11; j++) {

            terrain->pathLayer.set_value(j, i, Colors::Black);
        }

    }

  /*  for (int i = 0; i < 20; i++) {

        for (int j = 8; j < 12; j++) {

            terrain->pathLayer.set_value(i, j, Colors::Black);
        }

    }*/


    terrain->pathLayer.set_value(1, 1, Colors::White);
    terrain->pathLayer.set_value(2, 1, Colors::White);

    terrain->pathLayer.set_value(5, 1, Colors::White);
    terrain->pathLayer.set_value(6, 1, Colors::White);

    terrain->pathLayer.set_value(9, 1, Colors::White);
    terrain->pathLayer.set_value(10, 1, Colors::White);

    terrain->pathLayer.set_value(13, 1, Colors::White);
    terrain->pathLayer.set_value(14, 1, Colors::White);

    terrain->pathLayer.set_value(17, 1, Colors::White);
    terrain->pathLayer.set_value(18, 1, Colors::White);




    terrain->pathLayer.set_value(1, 4, Colors::White);
    terrain->pathLayer.set_value(1, 5, Colors::White);

    terrain->pathLayer.set_value(1, 8, Colors::White);
    terrain->pathLayer.set_value(1, 9, Colors::White);

    terrain->pathLayer.set_value(1, 12, Colors::White);
    terrain->pathLayer.set_value(1, 13, Colors::White);

    terrain->pathLayer.set_value(1, 16, Colors::White);
    terrain->pathLayer.set_value(1, 17, Colors::White);


    // Camera position can be modified from this default
    auto camera = agents->get_camera_agent();
    camera->set_position(Vec3(-62.0f, 70.0f, terrain->mapSizeInWorld * 0.5f));
    camera->set_pitch(0.610865); // 35 degrees

    // Sound control (these sound functions can be kicked off in a behavior tree node - see the example in L_PlaySound.cpp)
    audioManager->SetVolume(0.5f);
    audioManager->PlaySoundEffect(L"Assets\\Audio\\retro.wav");
    // Uncomment for example on playing music in the engine (must be .wav)
    // audioManager->PlayMusic(L"Assets\\Audio\\motivate.wav");
    // audioManager->PauseMusic(...);
    // audioManager->ResumeMusic(...);
    // audioManager->StopMusic(...);
}