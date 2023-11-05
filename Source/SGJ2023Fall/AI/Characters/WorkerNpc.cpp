// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkerNpc.h"

#include "GameplayTagContainer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "SGJ2023Fall/Data/GameplayMessages.h"
#include "SGJ2023Fall/Settings/GameAISettings.h"

// Called when the game starts or when spawned
void AWorkerNpc::BeginPlay()
{
	Super::BeginPlay();
	auto AISettings = GetDefault<UGameAISettings>();
	if (AISettings->StopWorkingEventTag.IsValid())
		UGameplayMessageSubsystem::Get(this).RegisterListener(AISettings->StopWorkingEventTag,
			this, &AWorkerNpc::OnStopWorkingMessage);

	if (AISettings->ResumeWorkingEventTag.IsValid())
		UGameplayMessageSubsystem::Get(this).RegisterListener(AISettings->ResumeWorkingEventTag,
			this, &AWorkerNpc::OnResumeWorkingMessage);
}

void AWorkerNpc::OnStopWorkingMessage(FGameplayTag MessageTag, const FGameplayMessage_EmptyMessage& Message)
{
	SetAIWorkingEnabled(false);
}

void AWorkerNpc::OnResumeWorkingMessage(FGameplayTag MessageTag, const FGameplayMessage_EmptyMessage& Message)
{
	SetAIWorkingEnabled(true);
}

void AWorkerNpc::SetAIWorkingEnabled(bool bEnabled)
{
	if (auto Blackboard = GetController()->FindComponentByClass<UBlackboardComponent>())
	{
		auto AISettings = GetDefault<UGameAISettings>();		
		if (AISettings->StopWorkingBBKey.SelectedKeyName.IsValid())
			Blackboard->SetValueAsBool(AISettings->StopWorkingBBKey.SelectedKeyName, !bEnabled);
	}
}