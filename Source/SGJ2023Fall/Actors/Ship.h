// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Ship.generated.h"

class UBoxComponent;

USTRUCT(BlueprintType)
struct FShipRequest
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ItemTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int CountRequired = 1;
	
	int CurrentCount = 0;
};

USTRUCT(BlueprintType)
struct FShipEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FShipRequest> ItemsRequired;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeDelay = 10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ProgressGranted = 0.f;
};

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
