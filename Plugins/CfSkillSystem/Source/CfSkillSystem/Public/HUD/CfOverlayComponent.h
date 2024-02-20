// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CfOverlayComponent.generated.h"

/**
 * 
 */
UCLASS()
class CFSKILLSYSTEM_API UCfOverlayComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void ShowWidget(bool bShow);
};
