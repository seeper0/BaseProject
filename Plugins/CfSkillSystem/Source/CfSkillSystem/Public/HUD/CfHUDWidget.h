// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CfHUDWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class CFSKILLSYSTEM_API UCfHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

#pragma region COMMON
public:
	FVector2D GetTopLeft() const { return HUDTopLeft; }
	FVector2D GetBottomRight() const { return HUDBottomRight; }
	FVector2D ToAbsolute(const FVector2D& Local) const;

protected:
	FVector2D HUDSize;
	FVector2D HUDTopLeft;
	FVector2D HUDBottomRight;
#pragma endregion
	
#pragma region CROSSHAIR
public:
	FVector2D GetCrossHairScreenLocation() const;
	FVector2D GetHudCenterScreenLocation() const;

	UPROPERTY(meta = (BindWidget))
	class UCfCrossHairWidget* CrossHair = nullptr;
#pragma endregion

#pragma region TARGET
public:
	void RegisterTargetWidget(class UCfOverlayLockOnComponent* InTarget);
	void UnregisterTargetWidget();
	void ToggleTargetWidget(class UCfOverlayLockOnComponent* InTarget);
	class UCfOverlayLockOnComponent* GetLockingTarget() const { return LockingTarget; }

private:
	UPROPERTY()
	class UCfOverlayLockOnComponent* LockingTarget;
#pragma endregion
};
