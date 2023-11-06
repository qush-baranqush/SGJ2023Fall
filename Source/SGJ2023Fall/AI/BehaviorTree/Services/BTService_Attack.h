// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "UObject/Object.h"
#include "BTService_Attack.generated.h"

/**
 * 
 */
UCLASS()
class SGJ2023FALL_API UBTService_Attack : public UBTService
{
	GENERATED_BODY()

private:
	struct FBTMemory_Attack : public FBTAuxiliaryMemory
	{
		float TimeToNextAttack = 0.f;
	};
	
public:
	UBTService_Attack();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTMemory_Attack); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackDistance = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin = 0.f, ClampMin = 0.f, UIMax = 1.f, ClampMax = 1.f))
	float ContinueComboChance = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackCooldown = 2.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector TargetBBKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector AttackOnCooldownBBKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector IsAttackingBBKey;
};
