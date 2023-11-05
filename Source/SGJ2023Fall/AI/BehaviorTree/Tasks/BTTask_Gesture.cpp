// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Gesture.h"

#include "AIController.h"
#include "SGJ2023Fall/AI/GestureComponent.h"

UBTTask_Gesture::UBTTask_Gesture()
{
	NodeName = "Gesture";
}

EBTNodeResult::Type UBTTask_Gesture::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto GestureComponent = OwnerComp.GetAIOwner()->GetPawn()->FindComponentByClass<UGestureComponent>();
	if (!GestureComponent)
		return EBTNodeResult::Failed;

	bool bStartedGesture = GestureComponent->TryStartGesture(GestureTag);
	return bStartedGesture ? EBTNodeResult::InProgress : EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_Gesture::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto GestureComponent = OwnerComp.GetAIOwner()->GetPawn()->FindComponentByClass<UGestureComponent>())
	{
		GestureComponent->TryStopGesture();
	}
	
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_Gesture::OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID,
                                bool bSuccess)
{
	if (Message == GestureTag.GetTagName())
	{
		Super::OnMessage(OwnerComp, NodeMemory, Message, RequestID, bSuccess);
	}
}

FString UBTTask_Gesture::GetStaticDescription() const
{
	return GestureTag.GetTagName().ToString();
}
