// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameModeBase.h"
#include "GJGameMode.generated.h"

class UWorldTimeComponent;
UCLASS()
class SGJ2023FALL_API AGJGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGJGameMode();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWorldTimeComponent* WorldTimeComponent;

};
