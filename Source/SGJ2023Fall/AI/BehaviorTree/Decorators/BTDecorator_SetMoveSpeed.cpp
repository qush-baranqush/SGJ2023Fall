// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_SetMoveSpeed.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTDecorator_SetMoveSpeed::UBTDecorator_SetMoveSpeed()
{
	NodeName = "Set move speed";
	bNotifyActivation = true;
	bNotifyDeactivation = true;
}

void UBTDecorator_SetMoveSpeed::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	Super::OnNodeActivation(SearchData);
	
	if (auto Pawn = Cast<ACharacter>(SearchData.OwnerComp.GetAIOwner()->GetPawn()))
	{
		auto MovementComponent = Pawn->GetCharacterMovement();
		FBTMemory_SetMoveSpeed* NodeMemory = GetNodeMemory<FBTMemory_SetMoveSpeed>(SearchData);
		NodeMemory->InitialSpeed = MovementComponent->MaxWalkSpeed;
		MovementComponent->MaxWalkSpeed = Speed;
	}
}

void UBTDecorator_SetMoveSpeed::OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult)
{
	if (auto Pawn = Cast<ACharacter>(SearchData.OwnerComp.GetAIOwner()->GetPawn()))
	{
		auto MovementComponent = Pawn->GetCharacterMovement();
		FBTMemory_SetMoveSpeed* NodeMemory = GetNodeMemory<FBTMemory_SetMoveSpeed>(SearchData);
		MovementComponent->MaxWalkSpeed = NodeMemory->InitialSpeed;
	}
	
	Super::OnNodeDeactivation(SearchData, NodeResult);
}
