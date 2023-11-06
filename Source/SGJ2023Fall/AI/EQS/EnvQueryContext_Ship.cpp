// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryContext_Ship.h"

#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "Kismet/GameplayStatics.h"
#include "SGJ2023Fall/Actors/Ship.h"

void UEnvQueryContext_Ship::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	AShip* Ship = Cast<AShip>(UGameplayStatics::GetActorOfClass(this, AShip::StaticClass()));
	if (Ship)
	{
		TArray<FVector> Locations = Ship->GetPierceLocations();
		for (FVector& Location : Locations)
		{
			Location += Ship->GetActorLocation();
		}
		
		UEnvQueryItemType_Point::SetContextHelper(ContextData, Locations);
	}
}
