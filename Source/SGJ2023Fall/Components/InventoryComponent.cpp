// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "GameFramework/GameplayMessageSubsystem.h"
#include "SGJ2023Fall/Actors/InteractableActor.h"
#include "SGJ2023Fall/Data/GameplayMessages.h"
#include "SGJ2023Fall/Settings/GameplaySettings.h"

void UInventoryComponent::PickUp(AInteractableActor* Actor)
{
	FGameplayMessage_ItemPickedUp MessageData;
	MessageData.ItemTag = Actor->GetItemTag();
	const UGameplaySettings* GameplaySettings = GetDefault<UGameplaySettings>();
	if (GameplaySettings->ItemAcquiredMessageTag.IsValid())
	{
		int& InventoryItemCount = Inventory.FindOrAdd(Actor->GetItemTag(), 1);
		InventoryItemCount++;
		MessageData.ItemsCount = InventoryItemCount;
		
		UGameplayMessageSubsystem::Get(this).BroadcastMessage(GameplaySettings->ItemAcquiredMessageTag, MessageData);
		Actor->OnPickedUp(); // TODO hang it on player
	}
}

void UInventoryComponent::Remove(const FGameplayTag& ItemTag, int RemoveCount)
{
	int* InventoryItemCount = Inventory.Find(ItemTag);
	if (InventoryItemCount)
	{
		*InventoryItemCount-=RemoveCount;
		if (*InventoryItemCount <= 0)
		{
			Inventory.Remove(ItemTag);
		}

		const UGameplaySettings* GameplaySettings = GetDefault<UGameplaySettings>();
		if (GameplaySettings->ItemAcquiredMessageTag.IsValid())
		{
			FGameplayMessage_ItemPickedUp MessageData;
			MessageData.ItemTag = ItemTag;
			MessageData.ItemsCount = *InventoryItemCount;
			UGameplayMessageSubsystem::Get(this).BroadcastMessage(GameplaySettings->ItemAcquiredMessageTag, MessageData);
		}
	}
}
