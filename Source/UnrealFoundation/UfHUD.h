// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UfHudDebuggable.h"
#include "UfHUD.generated.h"

/**
 * 
 */
UCLASS()
class UNREALFOUNDATION_API AUfHUD : public AHUD, public IUfHudDebuggable
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

#pragma region PlayerInfo
public:
	void TogglePlayerInfo();
private:
	void DrawPlayerInfo();
	bool bShowPlayerInfo = false;
#pragma endregion
};
