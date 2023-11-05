// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include "GameFramework/Character.h"
#include "WorkerNpc.generated.h"

struct FGameplayMessage_EmptyMessage;
struct FGameplayTag;
UCLASS()
class SGJ2023FALL_API AWorkerNpc : public AAICharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void OnStopWorkingMessage(FGameplayTag MessageTag, const FGameplayMessage_EmptyMessage& Message);
	void OnResumeWorkingMessage(FGameplayTag MessageTag, const FGameplayMessage_EmptyMessage& Message);
	void SetAIWorkingEnabled(bool bStopWorking);

};
