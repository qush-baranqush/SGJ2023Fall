// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "UObject/Object.h"
#include "BTDecorator_SetMoveSpeed.generated.h"

/**
 * 
 */
UCLASS()
class SGJ2023FALL_API UBTDecorator_SetMoveSpeed : public UBTDecorator
{
	GENERATED_BODY()

private:
	struct FBTMemory_SetMoveSpeed
	{
		float InitialSpeed = 0.f;
	};
	
public:
	UBTDecorator_SetMoveSpeed();
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTMemory_SetMoveSpeed); }

protected:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;
	virtual void OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Speed = 200.f;
};
