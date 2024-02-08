// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "CfCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class CFCOMMON_API UCfCheatManager : public UCheatManager
{
	GENERATED_BODY()

#pragma region PlayerInfo
public:
	UFUNCTION(Exec)
	void TogglePlayerInfo() { bShowPlayerInfo = !bShowPlayerInfo; }
	bool IsShowPlayerInfo() const { return bShowPlayerInfo; }
private:
	bool bShowPlayerInfo = false;
#pragma endregion

#pragma region HitBox
public:
	UFUNCTION(Exec)
	void ToggleHitBox() { bShowHitBox = !bShowHitBox; }
	bool IsShowHitBox() const { return bShowHitBox; }
private:
	bool bShowHitBox = false;
#pragma endregion

};
