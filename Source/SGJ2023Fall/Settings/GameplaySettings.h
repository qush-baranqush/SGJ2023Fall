// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GameplaySettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, defaultconfig)
class SGJ2023FALL_API UGameplaySettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	float TotalPlayTime = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	FRotator StartRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	FRotator EndRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	FName DirectionalLightTag = "DayLight";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	FGameplayTag EndgameTag;	
};
