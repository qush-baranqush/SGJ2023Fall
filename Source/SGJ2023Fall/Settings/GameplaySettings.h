// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GameplaySettings.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FGameTimeEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FGameplayTag> EventTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AtTime = -1.f;
};

UCLASS(Config=Game, defaultconfig)
class SGJ2023FALL_API UGameplaySettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	bool bDayNightChangeEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	float TotalPlayTime = 180.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	TArray<FRotator> SunlightPositions;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	FName DirectionalLightTag = "DayLight";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	FGameplayTag EndgameTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	FGameplayTag ShipBuiltTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	FGameplayTag ItemAcquiredMessageTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	FGameplayTag ShipRequestTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	TArray<FGameTimeEvent> GameTimeEvents;
	
};
