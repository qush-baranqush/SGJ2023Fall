// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "GesturesDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FGestureItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AttachmentSocket = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* StaticMesh = nullptr;
};

USTRUCT(BlueprintType)
struct FNpcGesture
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* GestureMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName MainLoopMontageSection = FName("MontageLoop");
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName EndLoopMontageSection = FName("MontageEndLoop");
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FGestureItem> GestureItems;

	// Key - section name of an intermediate montage, value - weight 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 0.001f, UIMin = 0.001f))
	TMap<FName, float> IntermediateSectionsWeights;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 0.001f, UIMin = 0.001f, EditCondition="IntermediateSections.Num() > 0"))
	float MainLoopWeight = 5.f;
    
	// suitable for montages that make the character to stick out of capsule too much. like sitting on a bench
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bDisableCollision = false;

	// Is montage supposed to be looped (kinda hacky but perhaps cheaper than looking for a loop in montage sections)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bLooped = false;
};

USTRUCT(BlueprintType)
struct FNpcGestures
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FNpcGesture> Gestures;
};

UCLASS()
class SGJ2023FALL_API UGesturesDataAsset : public UDataAsset
{
	GENERATED_BODY()
    
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FNpcGestures> GesturesOptions;
};
