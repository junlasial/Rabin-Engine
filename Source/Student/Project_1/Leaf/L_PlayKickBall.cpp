#include <pch.h>
#include "L_PlayKickBall.h"

void L_PlayKickBall::on_enter()
{


	audioManager->SetVolume(1.0f);
	audioManager->PlaySoundEffect(L"Assets\\Audio\\kickball.wav");
	BehaviorNode::on_leaf_enter();
	on_success();
}