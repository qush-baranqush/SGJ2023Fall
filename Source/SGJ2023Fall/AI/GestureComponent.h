// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BehaviorTree/Tasks/BTTask_Gesture.h"
#include "Components/ActorComponent.h"
#include "Data/GesturesDataAsset.h"
#include "GestureComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGestureCompleted, const FGameplayTag& GestureTag, bool bInterupted);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SGJ2023FALL_API UGestureComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	virtual bool TryStartGesture(const FGameplayTag& GestureTag);
	virtual bool TryStopGesture();
	const FGameplayTag& GetActiveGestureTag() const;
	void AttachGestureItem(const FGameplayTag& ItemTag, bool bAttach);
	void OnMontageSectionEnded() const;

	mutable FOnGestureCompleted OnGestureCompletedDelegate;
	
protected:
	virtual void BeginPlay() override;
	virtual const UGesturesDataAsset* GetGestures() const { return Gestures; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UGesturesDataAsset* Gestures;
		
	TWeakObjectPtr<ACharacter> CharacterOwner;

	FGameplayTag ActiveGestureTag;
	int ActiveGestureIndex = -1;
	
	UPROPERTY()
	TMap<FGameplayTag, UStaticMeshComponent*> GestureAttachedItems;

private:
	ECollisionEnabled::Type InitialMeshCollisionEnabledType;
	ECollisionEnabled::Type InitialCapsuleCollisionEnabledType;

	bool GetActiveGesture(FNpcGesture& OutGesture) const;
	void OnGestureMontageCompleted();

	void StopMontage();
	void ClearGestureItems();
	void ResetGestureState();

	FTimerHandle GestureTimer;
};
