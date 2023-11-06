// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"

#include "AIHealthComponent.h"
#include "CombatComponent.h"
#include "MotionWarpingComponent.h"
#include "SGJ2023Fall/AI/GestureComponent.h"


AAICharacter::AAICharacter()
{
	GestureComponent = CreateDefaultSubobject<UGestureComponent>(TEXT("Gesture"));
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	HealthComponent = CreateDefaultSubobject<UAIHealthComponent>(TEXT("HealthComponent"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
}