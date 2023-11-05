// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayBehaviorConfig_PlayMontage.h"

#include "GameplayBehavior_Gesture.h"

UGameplayBehaviorConfig_PlayMontage::UGameplayBehaviorConfig_PlayMontage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BehaviorClass = UGameplayBehavior_Gesture::StaticClass();
}
