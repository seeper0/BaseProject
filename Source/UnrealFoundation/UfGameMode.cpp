// Copyright Epic Games, Inc. All Rights Reserved.

#include "UfGameMode.h"
#include "UfHUD.h"
#include "UfPlayerController.h"
#include "UfCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUfGameMode::AUfGameMode()
{
	HUDClass = AUfHUD::StaticClass();

	PlayerControllerClass = AUfPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UCfHUDWidget> HudWidgetBPClass(TEXT("/Game/ThirdPerson/Blueprints/WBP_HUD"));
	if (HudWidgetBPClass.Class != nullptr)
	{
		HudWidgetClass = HudWidgetBPClass.Class;
	}
}
