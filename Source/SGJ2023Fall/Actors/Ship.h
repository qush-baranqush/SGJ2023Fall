﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGJ2023Fall/Data/ShipData.h"
#include "GameFramework/Actor.h"
#include "Ship.generated.h"

class UBoxComponent;

UCLASS()
class SGJ2023FALL_API AShip : public AActor
{
	GENERATED_BODY()

public:
	AShip();
	void Interact(ACharacter* Interactor);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FShipEvent> ShipEvents;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ShipMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* InteractionVolumeComponent;

private:
	int CurrentEventIndex = 0;
	FTimerHandle Timer;
	TArray<FShipRequest> CurrentRequests;
	
	void OnNewRequirement();
};
