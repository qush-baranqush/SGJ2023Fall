// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Actor.h"
#include "SGJ2023Fall/AI/Characters/AICharacter.h"
#include "SGJ2023Fall/Data/GameplayMessages.h"
#include "MobSpawner.generated.h"

UCLASS()
class SGJ2023FALL_API AMobSpawner : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EventTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int EnemiesPerTimeMin = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int EnemiesPerTimeMax = 4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CooldownBetweenSpawns = 1.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAICharacter> MobClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBehaviorTree* BT;
	
private:
	void SpawnEnemies(FGameplayTag MessageTag, const FGameplayMessage_EmptyMessage& MessagePayload);
	int MobsToSpawn = 0;
	float SpawnCooldown = 0.f;
};
