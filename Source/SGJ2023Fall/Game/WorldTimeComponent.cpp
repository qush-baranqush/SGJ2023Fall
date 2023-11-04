// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldTimeComponent.h"

#include "Engine/DirectionalLight.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "SGJ2023Fall/Data/GameplayMessages.h"
#include "SGJ2023Fall/Settings/GameplaySettings.h"


UWorldTimeComponent::UWorldTimeComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UWorldTimeComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> DirectionalLights;
	const UGameplaySettings* GameplaySettings = GetDefault<UGameplaySettings>();
	UGameplayStatics::GetAllActorsOfClass(this, ADirectionalLight::StaticClass(), DirectionalLights);
	for (auto* DirectionLightActor : DirectionalLights)
	{
		if (DirectionLightActor->ActorHasTag(GameplaySettings->DirectionalLightTag))
		{
			DirectionalLight = Cast<ADirectionalLight>(DirectionLightActor);
			break;
		}
	}

	check(DirectionalLight.Get());
	DirectionalLight->SetActorRotation(GameplaySettings->StartRotation);
}


// Called every frame
void UWorldTimeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const UGameplaySettings* GameplaySettings = GetDefault<UGameplaySettings>();
	if (AccumulatedTime < GameplaySettings->TotalPlayTime)
	{
		if (DirectionalLight.IsValid())
		{
			DirectionalLight->SetActorRotation(FMath::Lerp(GameplaySettings->StartRotation, GameplaySettings->EndRotation,
				AccumulatedTime / GameplaySettings->TotalPlayTime));
		}

		AccumulatedTime += DeltaTime;
	}
	else
	{
		SetComponentTickEnabled(false);
		if (GameplaySettings->EndgameTag.IsValid())
		{
			FGameplayMessage_Endgame Message = FGameplayMessage_Endgame();
			UGameplayMessageSubsystem::Get(this).BroadcastMessage(GameplaySettings->EndgameTag, Message);
		}
	}
	
}

