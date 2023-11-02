// Copyright Epic Games, Inc. All Rights Reserved.

#include "SGJ2023FallGameMode.h"
#include "SGJ2023FallCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASGJ2023FallGameMode::ASGJ2023FallGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
