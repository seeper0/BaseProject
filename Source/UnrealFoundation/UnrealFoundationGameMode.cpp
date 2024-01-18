// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealFoundationGameMode.h"
#include "UfHUD.h"
#include "UfPlayerController.h"
#include "UnrealFoundationCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealFoundationGameMode::AUnrealFoundationGameMode()
{
	HUDClass = AUfHUD::StaticClass();

	PlayerControllerClass = AUfPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUfHUDWidget> HudWidgetBPClass(TEXT("/Game/ThirdPerson/Blueprints/WBP_HUD"));
	if (HudWidgetBPClass.Class != nullptr)
	{
		HudWidgetClass = HudWidgetBPClass.Class;
	}
}
