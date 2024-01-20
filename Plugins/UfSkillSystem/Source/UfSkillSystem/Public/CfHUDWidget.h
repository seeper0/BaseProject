// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CfHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UFSKILLSYSTEM_API UCfHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class UCfCrossHairWidget* CrossHair = nullptr;
};
