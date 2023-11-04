// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "GameEventsManagerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SGJ2023FALL_API UGameEventsManagerComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	TArray<FTimerHandle> Timers;
	
	void OnEventHappen(TArray<FGameplayTag> EventsTags);
};
