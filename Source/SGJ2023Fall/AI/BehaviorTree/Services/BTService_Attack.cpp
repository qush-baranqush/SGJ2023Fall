// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Attack.h"

#include <Components/HealthComponent.h>

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SGJ2023Fall/AI/Characters/CombatComponent.h"

UBTService_Attack::UBTService_Attack()
{
	NodeName = "Attack";
	AttackOnCooldownBBKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_Attack, AttackOnCooldownBBKey));
	IsAttackingBBKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_Attack, IsAttackingBBKey));
}

void UBTService_Attack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsAttackingBBKey.SelectedKeyName, false);
	FBTMemory_Attack* AttackMemory = reinterpret_cast<FBTMemory_Attack*>(NodeMemory);

	auto Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetBBKey.SelectedKeyName));
	if (!Target)
		return;

	if (auto TargetHealthComponent = Target->FindComponentByClass<UHealthComponent>())
	{
		if (!TargetHealthComponent->IsAlive())
			return;
	}

	if (AttackMemory->TimeToNextAttack <= 0.f)
	{
		const float DistanceSq = FVector::DistSquared(Target->GetActorLocation(), OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation());
		if (DistanceSq > AttackDistance * AttackDistance)
			return;
		
		UCombatComponent* CombatComponent = OwnerComp.GetAIOwner()->GetPawn()->FindComponentByClass<UCombatComponent>();
		float AttackDuration = CombatComponent->Attack();
		
		if (AttackDuration < 0.f)
			return;

		SetNextTickTime(NodeMemory, AttackDuration * 0.8f);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsAttackingBBKey.SelectedKeyName, true);
		if (FMath::RandRange(0.f, 1.f) > ContinueComboChance)
		{
			AttackMemory->TimeToNextAttack += AttackCooldown;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(AttackOnCooldownBBKey.SelectedKeyName, true);
		}
	}
	else
	{
		AttackMemory->TimeToNextAttack -= DeltaSeconds;
		if (AttackMemory->TimeToNextAttack <= 0)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(AttackOnCooldownBBKey.SelectedKeyName, false); // TODO too often, 
		}
	}
}
