// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealFoundationGameMode.h"
#include "UfHud.h"
#include "UnrealFoundationCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealFoundationGameMode::AUnrealFoundationGameMode()
{
#if !UE_BUILD_SHIPPING
	HUDClass = AUfHud::StaticClass();
#endif

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
