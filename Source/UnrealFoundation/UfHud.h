// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UfHudDebuggable.h"
#include "UfHud.generated.h"

/**
 * 
 */
UCLASS()
class UNREALFOUNDATION_API AUfHud : public AHUD, public IUfHudDebuggable
{
	GENERATED_BODY()
	
	virtual void DrawHUD() override;
};
