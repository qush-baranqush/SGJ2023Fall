// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

USTRUCT(BlueprintType)
struct FComboAttackData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Range = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Radius = 25.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ResetComboAfter = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MontagePlayRate = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PushbackImpulseStrength = 20.f;
};

USTRUCT(BlueprintType)
struct FCombo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FComboAttackData> Attacks;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SGJ2023FALL_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	float Attack();

	void ApplyDamage();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FCombo> Combos;

private:
	FTimerHandle ComboTimer;
	FCombo* ActiveCombo = nullptr;
	int NextComboAttackIndex = 0;
	
	TWeakObjectPtr<ACharacter> CharacterOwner;

	void ResetCombo();
};
