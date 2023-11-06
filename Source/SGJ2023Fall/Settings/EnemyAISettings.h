// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardAssetProvider.h"
#include "UObject/Object.h"
#include "EnemyAISettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, defaultconfig)
class SGJ2023FALL_API UEnemyAISettings : public UDeveloperSettings, public IBlackboardAssetProvider
{
	GENERATED_BODY()

public:
	virtual UBlackboardData* GetBlackboardAsset() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	TSoftObjectPtr<UBlackboardData> BlackboardData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	FBlackboardKeySelector HealthBBKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	FBlackboardKeySelector IsAttackedBBKey;
};
