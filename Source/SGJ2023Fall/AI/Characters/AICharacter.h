// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

class UCombatComponent;
class UAIHealthComponent;
class UMotionWarpingComponent;
class UGestureComponent;
class UGesturesDataAsset;

UCLASS()
class SGJ2023FALL_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGestureComponent* GestureComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMotionWarpingComponent* MotionWarpingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCombatComponent* CombatComponent;	
};
