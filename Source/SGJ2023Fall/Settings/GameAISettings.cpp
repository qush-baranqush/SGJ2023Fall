// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAISettings.h"

UBlackboardData* UGameAISettings::GetBlackboardAsset() const
{
	return BlackboardData.LoadSynchronous();
}
