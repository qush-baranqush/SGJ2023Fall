// Fill out your copyright notice in the Description page of Project Settings.


#include "MobSpawner.h"

#include "AIController.h"
#include "AIHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameplayMessageSubsystem.h"

// Called when the game starts or when spawned
void AMobSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (BT)
	{
		SetActorTickEnabled(false);
		return;
	}
	
	UGameplayMessageSubsystem::Get(this).RegisterListener(EventTag, this, &AMobSpawner::SpawnEnemies);
}

// Called every frame
void AMobSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (MobsToSpawn > 0)
	{
		if (SpawnCooldown <= 0.f)
		{
			FTransform SpawnTransform(GetActorLocation() + FVector::UpVector * 100.f);
			AAICharacter* SpawnedMob = GetWorld()->SpawnActor<AAICharacter>(MobClass, SpawnTransform);
			if (auto AIController = Cast<AAIController>(SpawnedMob->GetController()))
			{
				AIController->RunBehaviorTree(BT);
			}
			
			MobsToSpawn--;
			SpawnCooldown = CooldownBetweenSpawns;
		}
		else
		{
			SpawnCooldown-=DeltaTime;
		}
	}
}

void AMobSpawner::SpawnEnemies(FGameplayTag MessageTag, const FGameplayMessage_EmptyMessage& MessagePayload)
{
	if (MessageTag != EventTag)
	{
		return;
	}

	MobsToSpawn = FMath::RandRange(EnemiesPerTimeMin, EnemiesPerTimeMax);
}