// 

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "UObject/Object.h"
#include "BTService_UpdatePerception.generated.h"

/**
 * 
 */
UCLASS()
class SGJ2023FALL_API UBTService_UpdatePerception : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_UpdatePerception();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector TargetBBKey;
};
