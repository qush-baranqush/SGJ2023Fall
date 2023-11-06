// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAISettings.h"
#include "BehaviorTree/BlackboardData.h"

UBlackboardData* UEnemyAISettings::GetBlackboardAsset() const
{
	return BlackboardData.LoadSynchronous();
}
