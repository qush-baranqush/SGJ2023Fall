// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "BTTask_FindAndUseSmartObject_Enhanced.generated.h"


class UAITask_UseGameplayBehaviorSmartObject;
class AITask_UseSmartObject;
struct FSmartObjectClaimHandle;

UCLASS()
class SGJ2023FALL_API UBTTask_FindAndUseSmartObject_Enhanced : public UBTTaskNode
{
	GENERATED_BODY()

private:
	struct FBTUseSOTaskMemory
	{
		TWeakObjectPtr<UAITask_UseGameplayBehaviorSmartObject> TaskInstance;
		int32 EQSRequestID;
	};

public:
	UBTTask_FindAndUseSmartObject_Enhanced();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTUseSOTaskMemory); }

	virtual FString GetStaticDescription() const override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	void OnQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	void UseClaimedSmartObject(UBehaviorTreeComponent& OwnerComp, FSmartObjectClaimHandle ClaimHandle, FBTUseSOTaskMemory& MyMemory);

protected:
	/** Additional tag query to filter available smart objects. We'll query for smart
	 *	objects that support activities tagged in a way matching the filter.
	 *	Note that regular tag-base filtering is going to take place as well */
	UPROPERTY(EditAnywhere, Category = SmartObjects)
	FGameplayTagQuery ActivityRequirements;

	UPROPERTY(EditAnywhere, Category = SmartObjects)
	FEQSParametrizedQueryExecutionRequest EQSRequest;

	/** Used for smart object querying if EQSRequest is not configured */
	UPROPERTY(EditAnywhere, Category = SmartObjects, meta=(DisplayName="Fallback Radius"))
	float Radius;

	FQueryFinishedSignature EQSQueryFinishedDelegate; 
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector SOClaimHandleBBKey;
};
