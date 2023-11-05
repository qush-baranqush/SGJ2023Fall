// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayBehaviorConfig.h"
#include "GameplayTagContainer.h"
#include "GameplayBehaviorConfig_PlayMontage.generated.h"

/**
 * 
 */
UCLASS()
class SGJ2023FALL_API UGameplayBehaviorConfig_PlayMontage : public UGameplayBehaviorConfig
{
	GENERATED_BODY()

public:
	UGameplayBehaviorConfig_PlayMontage(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag GestureTag;
};
