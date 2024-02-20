// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CfOverlayComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class CFSKILLSYSTEM_API UCfOverlayComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void ShowWidget(bool bShow);

protected:
	bool bShowWidget = false;
	float Progress = 0.0f;
	float HideTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float AlphaSpeed = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float HideDelayTime = 0.0f;
};
