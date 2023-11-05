// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayBehavior_Gesture.h"

#include "GameplayBehaviorConfig_PlayMontage.h"
#include "MotionWarpingComponent.h"
#include "SmartObjectPersistentCollection.h"
#include "SmartObjectRuntime.h"
#include "SmartObjectComponent.h"
#include "BlackboardKeyType_SOClaimHandle.h"
#include "SmartObjectSubsystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "SGJ2023Fall/AI/GestureComponent.h"
#include "SGJ2023Fall/Settings/GameAISettings.h"

bool UGameplayBehavior_Gesture::Trigger(AActor& Avatar, const UGameplayBehaviorConfig* Config,
                                        AActor* SmartObjectOwner)
{
	// auto Mesh = Avatar.FindComponentByClass<USkeletalMeshComponent>();
	// if (!Mesh)
	// 	return false;
	//
	UGestureComponent* GestureComponent = Avatar.FindComponentByClass<UGestureComponent>();
	if (!GestureComponent)
		return false;

	auto GestureConfig = Cast<UGameplayBehaviorConfig_PlayMontage>(Config);
	if (!GestureConfig)
		return false;

	if (!GestureConfig->GestureTag.IsValid())
		return false;
	
	if (auto MotionWarpingComponent = Avatar.FindComponentByClass<UMotionWarpingComponent>())
	{
		// TODO checks
		if (auto Blackboard = Cast<ACharacter>(&Avatar)->GetController()->FindComponentByClass<UBlackboardComponent>())
		{
			const UGameAISettings* GameAISettings = GetDefault<UGameAISettings>();
			if (GameAISettings->SmartObjectClaimHandleBBKey.SelectedKeyName.IsValid())
			{
				FSmartObjectClaimHandle SOClaimHandle = Blackboard->GetValue<UBlackboardKeyType_SOClaimHandle>(GameAISettings->SmartObjectClaimHandleBBKey.SelectedKeyName);
				auto SlotTransformOptional = USmartObjectSubsystem::GetCurrent(GetWorld())->GetSlotTransform(SOClaimHandle);
				if (SlotTransformOptional.IsSet())
				{
					const FName MotionWarpingName = "SmartObjectTransform"; // TODO parametrize/put to developer settings
					MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(MotionWarpingName, SlotTransformOptional.GetValue());
				}
			}
		}
	}
	
	bool bGestureStarted = GestureComponent->TryStartGesture(GestureConfig->GestureTag);
	if (!bGestureStarted)
		return false;

	AvatarActor = &Avatar;
	// TODO this is shit. Come up with an idea how to make it non instanced
	GestureComponent->OnGestureCompletedDelegate.AddUObject(this, &UGameplayBehavior_Gesture::OnGestureCompleted);
	Super::Trigger(Avatar, Config, SmartObjectOwner);
	return true;
}

void UGameplayBehavior_Gesture::OnGestureCompleted(const FGameplayTag& GestureTag, bool bInterupted)
{
	if (AvatarActor.IsValid())
		EndBehavior(*AvatarActor.Get(), false);
}

void UGameplayBehavior_Gesture::EndBehavior(AActor& Avatar, const bool bInterrupted)
{
	if (bInterrupted)
	{
		if (auto GestureComponent = Avatar.FindComponentByClass<UGestureComponent>())
		{
			GestureComponent->TryStopGesture();
		}
	}

	if (auto GestureComponent = Avatar.FindComponentByClass<UGestureComponent>())
	{
		GestureComponent->OnGestureCompletedDelegate.RemoveAll(this);
	}
	
	Super::EndBehavior(Avatar, bInterrupted);
}

bool UGameplayBehavior_Gesture::NeedsInstance(const UGameplayBehaviorConfig* Config) const
{
	return true;
}
