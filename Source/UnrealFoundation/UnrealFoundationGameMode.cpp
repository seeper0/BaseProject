// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealFoundationGameMode.h"
#include "UnrealFoundationCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealFoundationGameMode::AUnrealFoundationGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
