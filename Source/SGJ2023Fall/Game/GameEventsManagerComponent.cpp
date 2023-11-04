// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEventsManagerComponent.h"

#include "GameFramework/AsyncAction_ListenForGameplayMessage.h"
#include "SGJ2023Fall/Data/GameplayMessages.h"
#include "SGJ2023Fall/Settings/GameplaySettings.h"

// Called when the game starts
void UGameEventsManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	const auto GameplaySettings = GetDefault<UGameplaySettings>();
	for (const auto& GameEvent : GameplaySettings->GameTimeEvents)
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &UGameEventsManagerComponent::OnEventHappen, GameEvent.EventTag);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, GameEvent.AtTime, false);
		Timers.Add(TimerHandle);
	}
}

void UGameEventsManagerComponent::OnEventHappen(TArray<FGameplayTag> EventsTags)
{
	for (const auto& EventTag : EventsTags)
	{
		UGameplayMessageSubsystem::Get(this).BroadcastMessage(EventTag, FGameplayMessage_EmptyMessage());
	}
}

