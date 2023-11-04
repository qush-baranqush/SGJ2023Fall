// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "SGJ2023Fall/Components/InventoryComponent.h"
#include "SGJ2023Fall/Data/GameplayMessages.h"
#include "SGJ2023Fall/Settings/GameAISettings.h"
#include "SGJ2023Fall/Settings/GameplaySettings.h"

AShip::AShip()
{
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(ShipMesh);
	InteractionVolumeComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();
	if (ShipEvents.Num() <= 0)
		return;
	
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AShip::OnNewRequirement, ShipEvents[0].TimeDelay);	
}

void AShip::Interact(ACharacter* Interactor)
{
	UInventoryComponent* InventoryComponent = Interactor->FindComponentByClass<UInventoryComponent>();
	if (!InventoryComponent)
		return;
	TMap<FGameplayTag, int> ItemsToTake;
	TArray<FGameplayTag> RequestsToRemove;
	
	const auto Inventory = InventoryComponent->GetInventory();
	for (auto& Request : CurrentRequests)
	{
		const int* ItemCountPtr = Inventory.Find(Request.ItemTag);
		if (ItemCountPtr)
		{
			int QuantityToTake = FMath::Min(*ItemCountPtr, Request.CountRequired - Request.CurrentCount);
			Request.CurrentCount += QuantityToTake;
			ItemsToTake.Add(Request.ItemTag, QuantityToTake);
			if (Request.CurrentCount >= Request.CountRequired)
			{
				RequestsToRemove.Add(Request.ItemTag);
			}
		}
	}

	for (const auto& TakenItem : ItemsToTake)
	{
		InventoryComponent->Remove(TakenItem.Key, TakenItem.Value);
	}

	CurrentRequests.RemoveAll([RequestsToRemove](const FShipRequest& ShipRequest) { return RequestsToRemove.Contains(ShipRequest.ItemTag); } );
	if (CurrentRequests.Num() == 0)
	{
		if (ShipEvents.Num() < CurrentEventIndex)
		{
			const UGameAISettings* GameAISettings = GetDefault<UGameAISettings>();
			if (GameAISettings->ResumeWorkingEventTag.IsValid())
				UGameplayMessageSubsystem::Get(this).BroadcastMessage(GameAISettings->ResumeWorkingEventTag, FGameplayMessage_EmptyMessage());

			CurrentEventIndex++;
			GetWorld()->GetTimerManager().SetTimer(Timer, this, &AShip::OnNewRequirement, ShipEvents[CurrentEventIndex].TimeDelay, false);
		}
		else
		{
			const UGameplaySettings* GameplaySettings = GetDefault<UGameplaySettings>();
			if (GameplaySettings->ShipBuiltTag.IsValid())
				UGameplayMessageSubsystem::Get(this).BroadcastMessage(GameplaySettings->ShipBuiltTag, FGameplayMessage_EmptyMessage());
		}
	}
}

void AShip::OnNewRequirement()
{
	if (CurrentEventIndex >= ShipEvents.Num())
		return;
	
	CurrentRequests = ShipEvents[CurrentEventIndex].ItemsRequired;
	const UGameAISettings* GameAISettings = GetDefault<UGameAISettings>();
	if (GameAISettings->StopWorkingEventTag.IsValid())
		UGameplayMessageSubsystem::Get(this).BroadcastMessage(GameAISettings->StopWorkingEventTag, FGameplayMessage_EmptyMessage());

	const UGameplaySettings* GameplaySettings = GetDefault<UGameplaySettings>();
	if (GameplaySettings->ShipRequestTag.IsValid())
	{
		for (const auto& CurrentRequest : CurrentRequests)
		{
			FGameplayMessage_ShipItemRequest ShipItemRequest;
			ShipItemRequest.ItemsCount = CurrentRequest.CountRequired;
			ShipItemRequest.ItemTag = CurrentRequest.ItemTag;
			UGameplayMessageSubsystem::Get(this).BroadcastMessage(GameplaySettings->ShipRequestTag, ShipItemRequest);	
		}
	}
}