// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "CfCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class CFSKILLSYSTEM_API UCfCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
	UFUNCTION(Exec)
	void TogglePlayerInfo();
};
