﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS(BlueprintType)
class SGJ2023FALL_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:
	const FGameplayTag& GetItemTag() const { return ItemTag; }

	UFUNCTION(BlueprintNativeEvent)
	void SetActive(bool bActiveNew);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnPickedUp();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ItemTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer SpecificTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bActive = true;	
};
