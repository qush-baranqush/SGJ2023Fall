// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "EnemyAIController.generated.h"

UCLASS()
class SGJ2023FALL_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyAIController(const FObjectInitializer& ObjectInitializer);
};
