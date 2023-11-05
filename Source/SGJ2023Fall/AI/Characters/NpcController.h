// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "NpcController.generated.h"

UCLASS()
class SGJ2023FALL_API ANpcController : public AAIController
{
	GENERATED_BODY()

public:
	ANpcController(const FObjectInitializer& ObjectInitializer);
};
