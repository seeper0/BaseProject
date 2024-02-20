// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CfHUDWidget.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "HUD/CfCrossHairWidget.h"
#include "HUD/CfOverlayInfoComponent.h"
#include "HUD/CfOverlayLockOnComponent.h"
#include "CfLogger.h"

void UCfHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(GetWorld() && GetWorld()->GetGameViewport())
	{
		GetWorld()->GetGameViewport()->GetViewportSize(HUDSize);
		HUDTopLeft = GetCachedGeometry().LocalToAbsolute(FVector2D(0, 0));
		HUDBottomRight = GetCachedGeometry().LocalToAbsolute(GetCachedGeometry().GetLocalSize());
	}
}

FVector2D UCfHUDWidget::ToAbsolute(const FVector2D& Local) const
{
	return (Local * (HUDBottomRight - HUDTopLeft) / HUDSize) - HUDTopLeft;
}

FVector2D UCfHUDWidget::GetCrossHairScreenLocation() const
{
	if(CrossHair)
	{
		const FGeometry& CrossHairGeometry = CrossHair->GetCachedGeometry();
		const FVector2D LocalPoint = FVector2D(CrossHairGeometry.GetLocalSize() / 2.0f);
		const FVector2D CrossHairPoint = CrossHairGeometry.LocalToAbsolute(LocalPoint);

		const FVector2D AbsolutePoint = ToAbsolute(CrossHairPoint);
		//CF_LOG(TEXT("[%f,%f] %f,%f"), CrossHairPoint.X, CrossHairPoint.Y, AbsolutePoint.X, AbsolutePoint.Y);
		return AbsolutePoint;
	}
	return FVector2D();
}

FVector2D UCfHUDWidget::GetHudCenterScreenLocation() const
{
	const FGeometry& Geometry = GetCachedGeometry();
	const FVector2D LocalPoint = FVector2D(Geometry.GetLocalSize() / 2.0f);
	const FVector2D CenterPoint = Geometry.LocalToAbsolute(LocalPoint);
	return ToAbsolute(CenterPoint);
}

void UCfHUDWidget::RegisterTargetWidget(UCfOverlayLockOnComponent* InTarget)
{
	if(InTarget == nullptr)
		return;

	LockingTarget = InTarget;
	LockingTarget->ShowWidget(true);
	if(UCfOverlayInfoComponent* Info = LockingTarget->GetOwner()->GetComponentByClass<UCfOverlayInfoComponent>())
	{
		Info->ShowWidget(true);
	}
}

void UCfHUDWidget::UnregisterTargetWidget()
{
	if(LockingTarget == nullptr)
		return;

	LockingTarget->ShowWidget(false);
	if(UCfOverlayInfoComponent* Info = LockingTarget->GetOwner()->GetComponentByClass<UCfOverlayInfoComponent>())
	{
		Info->ShowWidget(false);
	}
	LockingTarget = nullptr;
}

void UCfHUDWidget::ToggleTargetWidget(UCfOverlayLockOnComponent* InTarget)
{
	if(LockingTarget)
	{
		UnregisterTargetWidget();
	}
	else
	{
		RegisterTargetWidget(InTarget);
	}
}
