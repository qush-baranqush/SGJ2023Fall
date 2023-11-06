// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIHealthComponent.generated.h"


class AAIController;
class AAICharacter;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SGJ2023FALL_API UAIHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHealth = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Health = 0.f;

protected:
	UFUNCTION()
	void OnDamageTaken(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName,
	                   FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);
	
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<AAICharacter> CharacterOwner;

	FTimerHandle ForgetWasAttackedTimer;
	
	void Die(float LastDamage, const FVector& LastHitDirection);
	void ForgetWasAttacked();
};