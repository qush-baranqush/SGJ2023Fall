// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldTimeComponent.h"

#include "Engine/DirectionalLight.h"
#include "GameFramework/Character.h"
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
	if (!GameplaySettings->bDayNightChangeEnabled)
	{
		SetComponentTickEnabled(false);
		return;
	}
	
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
	check(GameplaySettings->SunlightPositions.Num() > 1);
	DirectionalLight->SetActorRotation(GameplaySettings->SunlightPositions[0]);
	TimePerPosition = GameplaySettings->TotalPlayTime / (GameplaySettings->SunlightPositions.Num() - 1);

	if (GameplaySettings->ShipBuiltTag.IsValid())
	{
		UGameplayMessageSubsystem::Get(this).RegisterListener(GameplaySettings->ShipBuiltTag, this, &UWorldTimeComponent::OnShipBuilt);
	}
}


// Called every frame
void UWorldTimeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const UGameplaySettings* GameplaySettings = GetDefault<UGameplaySettings>();
	if (GameplaySettings->SunlightPositions.Num() < 2)
		return;
	
	if (TotalAccumulatedTime < GameplaySettings->TotalPlayTime)
	{
		if (DirectionalLight.IsValid())
		{
			FRotator NewRotation = FMath::Lerp(GameplaySettings->SunlightPositions[CurrentIndex], GameplaySettings->SunlightPositions[CurrentIndex+1],
				CurrentPeriodTime / TimePerPosition);
			UE_VLOG(this, LogTemp, Verbose, TEXT("Accumulated time = %.2f"), TotalAccumulatedTime);
			UE_VLOG(this, LogTemp, Verbose, TEXT("Current period time = %.2f"), CurrentPeriodTime);
			UE_VLOG(this, LogTemp, Verbose, TEXT("Current rotator index = %d"), CurrentIndex);
			UE_VLOG(this, LogTemp, Verbose, TEXT("Alpha = %.2f;"), CurrentPeriodTime / TimePerPosition);
			UE_VLOG(this, LogTemp, Verbose, TEXT("New rotation = %s;"), *NewRotation.ToString());
			UE_VLOG(this, LogTemp, Verbose, TEXT("Time per pos = %.2f;"), TimePerPosition);

			CurrentPeriodTime+=DeltaTime;
			if (CurrentPeriodTime > TimePerPosition)
			{
				CurrentIndex++;
				CurrentPeriodTime = 0.f;
			}
			
			DirectionalLight->SetActorRotation(NewRotation);
		}

		TotalAccumulatedTime += DeltaTime;
	}
	else
	{
		SetComponentTickEnabled(false);
		if (GameplaySettings->EndgameTag.IsValid())
		{
			FGameplayMessage_Endgame Message = FGameplayMessage_Endgame();
			UGameplayMessageSubsystem::Get(this).BroadcastMessage(GameplaySettings->EndgameTag, Message);
			auto PlayerController = UGameplayStatics::GetPlayerController(this, 0);
			auto PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
			PlayerCharacter->DisableInput(PlayerController);
			PlayerController->DisableInput(PlayerController);
		}
	}
}

void UWorldTimeComponent::OnShipBuilt(FGameplayTag MessageTag, const FGameplayMessage_EmptyMessage& Message)
{
	SetComponentTickEnabled(false);
}

