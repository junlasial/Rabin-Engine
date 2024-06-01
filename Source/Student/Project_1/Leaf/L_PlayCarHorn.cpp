#include <pch.h>
#include "L_PlayCarHorn.h"

void L_PlayCarHorn::on_enter()
{

	audioManager->SetVolume(1.0f);
	audioManager->PlaySoundEffect(L"Assets\\Audio\\horn.wav");




	BehaviorNode::on_leaf_enter();
	on_success();
}