// Fill out your copyright notice in the Description page of Project Settings.


#include "AIHealthComponent.h"

#include "AICharacter.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SGJ2023Fall/Settings/EnemyAISettings.h"
#include "SGJ2023Fall/Settings/GameAISettings.h"

void UAIHealthComponent::OnDamageTaken(AActor* DamagedActor, float Damage, AController* InstigatedBy,
                                       FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
                                       const UDamageType* DamageType, AActor* DamageCauser)
{
	Health -= Damage;
	const auto AISettings = GetDefault<UEnemyAISettings>();
	if (AISettings->HealthBBKey.SelectedKeyName.IsValid() && AISettings->IsAttackedBBKey.SelectedKeyName.IsValid())
	{
		if (auto AIController = CharacterOwner->GetController())
		{
			if (auto Blackboard = AIController->FindComponentByClass<UBlackboardComponent>())
			{
				Blackboard->SetValueAsFloat(AISettings->HealthBBKey.SelectedKeyName, Health);
				Blackboard->SetValueAsBool(AISettings->IsAttackedBBKey.SelectedKeyName, true);
				GetWorld()->GetTimerManager().ClearTimer(ForgetWasAttackedTimer);
				GetWorld()->GetTimerManager().SetTimer(ForgetWasAttackedTimer, this, &UAIHealthComponent::ForgetWasAttacked, 10.f, false);
			}
		}	
	}
	
	if (Health < 0.f)
	{
		Die(Damage, -ShotFromDirection);	
	}
}

void UAIHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CharacterOwner = Cast<AAICharacter>(GetOwner());
	CharacterOwner->OnTakePointDamage.AddDynamic(this, &UAIHealthComponent::OnDamageTaken);
	Health = MaxHealth;
	
	const auto AISettings = GetDefault<UEnemyAISettings>();
	if (AISettings->IsAttackedBBKey.SelectedKeyName.IsValid())
	{
		if (auto AIController = CharacterOwner->GetController())
		{
			if (auto Blackboard = AIController->FindComponentByClass<UBlackboardComponent>())
			{
				Blackboard->SetValueAsFloat(AISettings->HealthBBKey.SelectedKeyName, Health);
			}
		}	
	}
}

void UAIHealthComponent::Die(float LastDamage, const FVector& LastHitDirection)
{
	if (auto AIController = Cast<AAIController>(CharacterOwner->GetController()))
	{
		AIController->GetBrainComponent()->StopLogic("Dead");
	}
	
	CharacterOwner->GetMesh()->SetSimulatePhysics(true);
	CharacterOwner->GetMesh()->SetCollisionProfileName("Ragdoll");
	CharacterOwner->GetMesh()->AddImpulse(LastHitDirection * FMath::RandRange(50.f, 400.f) + FVector::UpVector * FMath::RandRange(50.f, 300.f));
	CharacterOwner->OnTakePointDamage.RemoveAll(this);
}

void UAIHealthComponent::ForgetWasAttacked()
{
	const auto AISettings = GetDefault<UEnemyAISettings>();
	if (AISettings->IsAttackedBBKey.SelectedKeyName.IsValid())
	{
		if (auto AIController = CharacterOwner->GetController())
		{
			if (auto Blackboard = AIController->FindComponentByClass<UBlackboardComponent>())
			{
				Blackboard->SetValueAsBool(AISettings->IsAttackedBBKey.SelectedKeyName, false);
			}
		}	
	}
}
