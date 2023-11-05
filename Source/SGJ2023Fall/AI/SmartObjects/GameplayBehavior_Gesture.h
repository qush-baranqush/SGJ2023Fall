// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayBehavior.h"
#include "GameplayBehavior_Gesture.generated.h"

/**
 * 
 */
UCLASS()
class SGJ2023FALL_API UGameplayBehavior_Gesture : public UGameplayBehavior
{
	GENERATED_BODY()

public:
	virtual bool Trigger(AActor& Avatar, const UGameplayBehaviorConfig* Config, AActor* SmartObjectOwner) override;
	virtual void EndBehavior(AActor& Avatar, const bool bInterrupted) override;
	virtual bool NeedsInstance(const UGameplayBehaviorConfig* Config) const override;
	
private:
	void OnGestureCompleted(const FGameplayTag& GestureTag, bool bInterupted);
	TWeakObjectPtr<AActor> AvatarActor;
};