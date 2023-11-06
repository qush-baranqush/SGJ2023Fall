// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ApplyDamage.h"

#include "CombatComponent.h"

void UAnimNotify_ApplyDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (auto CombatComponent = MeshComp->GetOwner()->FindComponentByClass<UCombatComponent>())
	{
		CombatComponent->ApplyDamage();
	}
}
