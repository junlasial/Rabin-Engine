#include <pch.h>
#include "L_PlayTraffic.h"

void L_PlayTraffic::on_enter()
{

	audioManager->SetVolume(1.0f);
	audioManager->PlaySoundEffect(L"Assets\\Audio\\trafficlight.wav");




	BehaviorNode::on_leaf_enter();
	on_success();
}