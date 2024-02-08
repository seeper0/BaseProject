// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HUD/CfHUDWidget.h"
#include "UfGameMode.generated.h"

UCLASS(minimalapi)
class AUfGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUfGameMode();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<UCfHUDWidget> HudWidgetClass;
};



