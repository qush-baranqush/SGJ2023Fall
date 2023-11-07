// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "BrainComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "SGJ2023Fall/Settings/GameplaySettings.h"


AEnemyAIController::AEnemyAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))

{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	auto GameplaySettings = GetDefault<UGameplaySettings>();
	if (GameplaySettings->EndgameTag.IsValid())
		UGameplayMessageSubsystem::Get(this).RegisterListener(GameplaySettings->EndgameTag, this, &AEnemyAIController::OnEndgame);

	if (GameplaySettings->ShipBuiltTag.IsValid())
		UGameplayMessageSubsystem::Get(this).RegisterListener(GameplaySettings->ShipBuiltTag, this, &AEnemyAIController::OnShipBuilt);
}

void AEnemyAIController::OnEndgame(FGameplayTag MessageTag, const FGameplayMessage_Endgame& Message)
{
	if (IsValid(BrainComponent))
		BrainComponent->StopLogic("Endgame");
}

void AEnemyAIController::OnShipBuilt(FGameplayTag MessageTag, const FGameplayMessage_EmptyMessage& Message)
{
	if (IsValid(BrainComponent))
		BrainComponent->StopLogic("Endgame");
}
