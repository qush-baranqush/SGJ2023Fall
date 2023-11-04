// Fill out your copyright notice in the Description page of Project Settings.


#include "GJGameMode.h"

#include "WorldTimeComponent.h"


// Sets default values
AGJGameMode::AGJGameMode()
{
	WorldTimeComponent = CreateDefaultSubobject<UWorldTimeComponent>(TEXT("WorldTime"));
}
