// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UGestureComponent* GestureComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMotionWarpingComponent* MotionWarpingComponent;
};
