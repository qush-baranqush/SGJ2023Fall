// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SGJ2023Fall/SGJ2023FallCharacter.h"

// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	CharacterOwner = Cast<ACharacter>(GetOwner());
}

float UCombatComponent::Attack()
{
	if (Combos.Num() <= 0 || !CharacterOwner.IsValid())
		return -1.f;

	float AttackDuration = -1.f;
	if (ActiveCombo == nullptr || NextComboAttackIndex <= 0 || (ActiveCombo && ActiveCombo->Attacks.Num() == NextComboAttackIndex))
	{
		ActiveCombo = &Combos[FMath::RandRange(0, Combos.Num() - 1)];
		NextComboAttackIndex = 0;
		if (ActiveCombo->Attacks.Num() < 0)
			return -1.f;
	}
	
	if (auto AnimInstance = CharacterOwner->GetMesh()->GetAnimInstance())
	{
		AttackDuration = AnimInstance->Montage_Play(ActiveCombo->Attacks[NextComboAttackIndex].Montage, ActiveCombo->Attacks[NextComboAttackIndex].MontagePlayRate);
	}

	if (AttackDuration > 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ComboTimer);
		GetWorld()->GetTimerManager().SetTimer(ComboTimer, this, &UCombatComponent::ResetCombo,
											   AttackDuration + ActiveCombo->Attacks[NextComboAttackIndex].ResetComboAfter);
	}
	
	NextComboAttackIndex++;
	return AttackDuration;
}

void UCombatComponent::ApplyDamage()
{
	FHitResult HitResult;
	const float CurrentIndex = NextComboAttackIndex - 1;
	const FVector StartLocation = CharacterOwner->GetActorLocation();
	const FVector EndLocation = CharacterOwner->GetActorLocation() + CharacterOwner->GetActorForwardVector() * ActiveCombo->Attacks[CurrentIndex].Range;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(ActiveCombo->Attacks[CurrentIndex].Radius);
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(CharacterOwner.Get());
	
	// TODO damage trace channel instead of visibility
	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, StartLocation, EndLocation, FQuat::Identity, ECC_Visibility,
		CollisionShape, CollisionQueryParams);
	if (bHit && HitResult.GetActor())
	{
		if (auto HitCharacter = Cast<ASGJ2023FallCharacter>(HitResult.GetActor()))
		{
			UGameplayStatics::ApplyPointDamage(HitCharacter, ActiveCombo->Attacks[CurrentIndex].Damage,
				CharacterOwner->GetActorForwardVector(), HitResult,  CharacterOwner->GetController(),
				CharacterOwner.Get(), UDamageType::StaticClass());
			FVector PushbackDirection = -HitResult.ImpactNormal;
			PushbackDirection.Z = FMath::RandRange(0.f, 0.5f);
			HitCharacter->GetCharacterMovement()->AddImpulse(-HitResult.ImpactNormal * ActiveCombo->Attacks[CurrentIndex].PushbackImpulseStrength);
		}
	}
}

void UCombatComponent::ResetCombo()
{
	ActiveCombo = nullptr;
	NextComboAttackIndex = -1;
}
