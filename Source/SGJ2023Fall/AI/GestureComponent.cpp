// Fill out your copyright notice in the Description page of Project Settings.


#include "GestureComponent.h"

#include "AIController.h"
#include "Characters/AICharacter.h"
#include "Components/CapsuleComponent.h"
#include "Data/GesturesDataAsset.h"


void UGestureComponent::BeginPlay()
{
	Super::BeginPlay();
	CharacterOwner = Cast<AAICharacter>(GetOwner());	
}

bool UGestureComponent::TryStartGesture(const FGameplayTag& GestureTag)
{
	if (IsValid(Gestures) == false)
	{
		return false;
	}

	const FNpcGestures* GestureOptions = Gestures->GesturesOptions.Find(GestureTag);
	if (GestureOptions == nullptr || GestureOptions->Gestures.Num() <= 0)
	{
		return false;
	}
	
	ActiveGestureIndex = FMath::RandRange(0, GestureOptions->Gestures.Num() - 1);
	const FNpcGesture& SelectedGesture = GestureOptions->Gestures[ActiveGestureIndex];
	ActiveGestureTag = GestureTag;

	if (SelectedGesture.GestureMontage)
	{
		float MontageDuration = CharacterOwner->PlayAnimMontage(SelectedGesture.GestureMontage);
		if (SelectedGesture.bDisableCollision)
		{
			// IDK for sure but I guess this will help the AI avoidance logic to ignore gesturing character's collision which can be useful in some cases
			InitialMeshCollisionEnabledType = CharacterOwner->GetMesh()->GetCollisionEnabled();
			InitialCapsuleCollisionEnabledType = CharacterOwner->GetCapsuleComponent()->GetCollisionEnabled();
			CharacterOwner->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			CharacterOwner->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		if (!SelectedGesture.bLooped)
		{
			GetWorld()->GetTimerManager().SetTimer(GestureTimer, this, &UGestureComponent::OnGestureMontageCompleted, MontageDuration, false);
		}
	}
	
	return true;
}

void UGestureComponent::StopMontage()
{
	const FNpcGestures* GestureOptions = Gestures->GesturesOptions.Find(ActiveGestureTag);
	if (GestureOptions == nullptr || GestureOptions->Gestures.Num() <= 0)
	{
		return;
	}
	
	const FNpcGesture& RunningGesture = GestureOptions->Gestures[ActiveGestureIndex];
	UAnimInstance* AnimInstance = CharacterOwner->GetMesh()->GetAnimInstance();
	if (IsValid(RunningGesture.GestureMontage) && AnimInstance->Montage_IsPlaying(RunningGesture.GestureMontage))
	{
		int32 EndSectionIndex = RunningGesture.GestureMontage->GetSectionIndex(RunningGesture.EndLoopMontageSection);
		if (EndSectionIndex != INDEX_NONE)
		{
			float EndGestureDelay = RunningGesture.GestureMontage->GetSectionLength(EndSectionIndex);
			AnimInstance->Montage_JumpToSection(RunningGesture.EndLoopMontageSection, RunningGesture.GestureMontage);
		}
		else
		{
			AnimInstance->Montage_Stop(0.2f, RunningGesture.GestureMontage);
		}

		if (RunningGesture.bDisableCollision)
		{
			CharacterOwner->GetMesh()->SetCollisionEnabled(InitialMeshCollisionEnabledType);
			CharacterOwner->GetCapsuleComponent()->SetCollisionEnabled(InitialCapsuleCollisionEnabledType);
		}
	}
}

void UGestureComponent::ClearGestureItems()
{
	if (GestureAttachedItems.Num() > 0)
	{
		for (const auto& ReactionAttachedItem : GestureAttachedItems)
		{
			ReactionAttachedItem.Value->DestroyComponent();
		}
        
		GestureAttachedItems.Reset();
	}
}

void UGestureComponent::ResetGestureState()
{
	ActiveGestureTag = FGameplayTag::EmptyTag;
	ActiveGestureIndex = -1;
}

bool UGestureComponent::TryStopGesture()
{
	if (ActiveGestureIndex < 0 || ActiveGestureTag.IsValid() == false)
	{
		return false;
	}

	if (IsValid(Gestures) == false)
	{
		return false;
	}

	StopMontage();
	ClearGestureItems();

	OnGestureCompletedDelegate.Broadcast(ActiveGestureTag, true);
	ResetGestureState();
	
    return true;
}

const FGameplayTag& UGestureComponent::GetActiveGestureTag() const
{
	return ActiveGestureTag;
}

void UGestureComponent::AttachGestureItem(const FGameplayTag& ItemTag, bool bAttach)
{
    FNpcGesture ActiveGesture;
	if (GetActiveGesture(ActiveGesture) == false)
	{
		return;
	}

    if (bAttach)
    {
    	const FGestureItem* GestureItem = ActiveGesture.GestureItems.Find(ItemTag);
        if (GestureItem == nullptr)
        {
	        return;
        }
    	
        UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(CharacterOwner.Get());
        StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
        StaticMeshComponent->RegisterComponent();
        StaticMeshComponent->AttachToComponent(CharacterOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, GestureItem->AttachmentSocket);
        StaticMeshComponent->SetStaticMesh(GestureItem->StaticMesh);

        if (UStaticMeshComponent** ExistingAttachedItem = GestureAttachedItems.Find(ItemTag))
        {
            (*ExistingAttachedItem)->DestroyComponent();
            GestureAttachedItems[ItemTag] = StaticMeshComponent;
        }
        else
        {
            GestureAttachedItems.Add(ItemTag, StaticMeshComponent);
        }
    }
    else
    {
	    bool bFound = false;
    	for (const auto& AttachedItem : GestureAttachedItems)
    	{
    		if (AttachedItem.Key == ItemTag)
    		{
    			AttachedItem.Value->DestroyComponent();
    			bFound = true;
    		}
    	}
        
    	if (bFound)
    	{
    		GestureAttachedItems.Remove(ItemTag);
    	}
    }
}

void UGestureComponent::OnMontageSectionEnded() const
{
	FNpcGesture ActiveGesture;
	if (GetActiveGesture(ActiveGesture) == false || ActiveGesture.IntermediateSectionsWeights.Num() <= 0)
	{
		return;
	}
	
	UAnimInstance* AnimInstance = CharacterOwner->GetMesh()->GetAnimInstance();
	const FName& CurrentSectionName = AnimInstance->Montage_GetCurrentSection(ActiveGesture.GestureMontage);

	float TotalWeight = ActiveGesture.MainLoopWeight;
	for (const auto& Section : ActiveGesture.IntermediateSectionsWeights)
	{
		TotalWeight += Section.Value;
	}

	float RandomWeight = FMath::RandRange(0.f, TotalWeight);
	if (RandomWeight <= ActiveGesture.MainLoopWeight)
	{
		if (CurrentSectionName != ActiveGesture.MainLoopMontageSection)
		{
			AnimInstance->Montage_SetNextSection(CurrentSectionName, ActiveGesture.MainLoopMontageSection, ActiveGesture.GestureMontage);
		}
		return;
	}

	RandomWeight -= ActiveGesture.MainLoopWeight;
	for (const auto& Section : ActiveGesture.IntermediateSectionsWeights)
	{
		if (RandomWeight <= Section.Value)
		{
			AnimInstance->Montage_SetNextSection(CurrentSectionName, Section.Key, ActiveGesture.GestureMontage);
			break;
		}

		RandomWeight -= Section.Value;
	}
}

bool UGestureComponent::GetActiveGesture(FNpcGesture& OutGesture) const
{
	if (ActiveGestureIndex < 0 || ActiveGestureTag.IsValid() == false)
	{
		return false;
	}
	
	if (IsValid(Gestures) == false)
	{
		return false;
	}
	
	const FNpcGestures& GestureOptions = Gestures->GesturesOptions[ActiveGestureTag];
	OutGesture = GestureOptions.Gestures[ActiveGestureIndex];
	return true;
}

void UGestureComponent::OnGestureMontageCompleted()
{
	if (auto AIController = Cast<AAIController>(CharacterOwner->GetController()))
	{
		if(UBrainComponent* Brain = AIController->GetBrainComponent())
		{
			FAIMessage Message;
			Message.MessageName = ActiveGestureTag.GetTagName();
			Message.Status = FAIMessage::Success;
			Brain->HandleMessage(Message);
		}
	}

	ClearGestureItems();
	OnGestureCompletedDelegate.Broadcast(ActiveGestureTag, false);
	ResetGestureState();
}
