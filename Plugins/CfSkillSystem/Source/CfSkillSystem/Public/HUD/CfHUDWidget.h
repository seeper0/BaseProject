// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CfMarkingComponent.h"
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
	float GetDPIScale() const { return DPIScale; }
	FVector2D GetStartPoint() const { return HUDStartPoint; }
protected:
	float DPIScale = 1.0f;
	FVector2D HUDStartPoint;
#pragma endregion
	
#pragma region CROSSHAIR
public:
	FVector2D GetCrossHairScreenLocation() const;

	UPROPERTY(meta = (BindWidget))
	class UCfCrossHairWidget* CrossHair = nullptr;
#pragma endregion

#pragma region TARGET
public:
	void RegisterTargetWidget(UCfMarkingComponent* InTarget);
	void UnregisterTargetWidget();
	void ToggleTargetWidget(UCfMarkingComponent* InTarget);
	UCfMarkingComponent* GetLockingTarget() const { return LockingTarget; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> TargetWidgetClass;

	UPROPERTY()
	UUserWidget* TargetMarkingWidget;

	UPROPERTY()
	UCfMarkingComponent* LockingTarget;
#pragma endregion
};
