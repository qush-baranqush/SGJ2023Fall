// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "SGJ2023Fall/Data/GameplayMessages.h"
#include "WorldTimeComponent.generated.h"


class ADirectionalLight;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SGJ2023FALL_API UWorldTimeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWorldTimeComponent();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	TWeakObjectPtr<ADirectionalLight> DirectionalLight;
	float TotalAccumulatedTime = 0.f;
	float CurrentPeriodTime = 0.f;
	float TimePerPosition = 0.f;
	int CurrentIndex = 0;
	void OnShipBuilt(FGameplayTag MessageTag, const FGameplayMessage_EmptyMessage& Message);
};
