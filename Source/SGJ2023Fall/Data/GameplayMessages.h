// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayMessages.generated.h"

USTRUCT(BlueprintType)
struct FGameplayMessage_Endgame
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FGameplayMessage_EmptyMessage
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FGameplayMessage_ItemPickedUp
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ItemTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ItemsCount = 0;	
};

USTRUCT(BlueprintType)
struct FGameplayMessage_ShipItemRequest
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ItemTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ItemsCount = 0;	
};