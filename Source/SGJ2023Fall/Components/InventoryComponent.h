// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AInteractableActor;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SGJ2023FALL_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void PickUp(AInteractableActor* Actor);
	// void Drop(AInteractableActor* Actor);
	// void Remove(AInteractableActor* Actor);

	void Remove(const FGameplayTag& ItemTag, int RemoveCount);
	
	const TMap<FGameplayTag, int>& GetInventory() const { return Inventory; }
	
protected:
	TMap<FGameplayTag, int> Inventory;
};
