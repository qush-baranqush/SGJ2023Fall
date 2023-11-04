// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardAssetProvider.h"
#include "BehaviorTree/BlackboardData.h"
#include "GameAISettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, defaultconfig)
class SGJ2023FALL_API UGameAISettings : public UDeveloperSettings, public IBlackboardAssetProvider
{
	GENERATED_BODY()

public:
	virtual UBlackboardData* GetBlackboardAsset() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	TSoftObjectPtr<UBlackboardData> BlackboardData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	FGameplayTag StopWorkingEventTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	FGameplayTag AttackedEventTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	FGameplayTag ResumeWorkingEventTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	FBlackboardKeySelector StopWorkingBBKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	FBlackboardKeySelector UnderAttackBBKey;
	
};
