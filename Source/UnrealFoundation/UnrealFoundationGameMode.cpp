// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealFoundationGameMode.h"
#include "UfHUD.h"
#include "UfPlayerController.h"
#include "UnrealFoundationCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealFoundationGameMode::AUnrealFoundationGameMode()
{
#if !UE_BUILD_SHIPPING
	HUDClass = AUfHUD::StaticClass();
#endif

	PlayerControllerClass = AUfPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
