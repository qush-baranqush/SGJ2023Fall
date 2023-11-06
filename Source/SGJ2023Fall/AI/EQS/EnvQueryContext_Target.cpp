// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryContext_Target.h"

#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UEnvQueryContext_Target::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	// TODO get player from ai controllers cached perception
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (Player)
	{
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, Player);
	}
}
