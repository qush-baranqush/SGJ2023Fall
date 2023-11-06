// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"


AEnemyAIController::AEnemyAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))

{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}
