// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CfHUDWidget.h"

#include "CfLogger.h"
#include "CfSkill.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "HUD/CfCrossHairWidget.h"

void UCfHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(GetWorld() && GetWorld()->GetGameViewport())
	{
		DPIScale = GetWorld()->GetGameViewport()->GetDPIScale();
		HUDStartPoint = GetCachedGeometry().LocalToAbsolute(FVector2D(0, 0)) * DPIScale;
	}

	if(TargetMarkingWidget)
	{
		if(LockingTarget)
		{
			if(APlayerController* PC = LockingTarget->GetWorld()->GetFirstPlayerController())
			{
				FVector2D ScreenLocation;
				if(PC->ProjectWorldLocationToScreen(LockingTarget->GetComponentLocation(), ScreenLocation))
				{
					TargetMarkingWidget->SetVisibility(ESlateVisibility::Visible);
					TargetMarkingWidget->SetPositionInViewport(ScreenLocation);
				}
				else
				{
					TargetMarkingWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
		else
		{
			UnregisterTargetWidget();
		}
	}
}

FVector2D UCfHUDWidget::GetCrossHairScreenLocation() const
{
	if(CrossHair)
	{
		const FGeometry& CrossHairGeometry = CrossHair->GetCachedGeometry();
		const FVector2D LocalPoint = FVector2D(CrossHairGeometry.GetLocalSize() / 2.0f);
		const FVector2D CrossHairPoint = CrossHairGeometry.LocalToAbsolute(LocalPoint) * DPIScale;

		const FVector2D AbsolutePoint = CrossHairPoint - GetStartPoint();
		//CF_LOG(TEXT("[%f,%f] %f,%f / %f,%f"), CrossHairPoint.X, CrossHairPoint.Y, ParentPoint.X, ParentPoint.Y, AbsolutePoint.X, AbsolutePoint.Y);
		return AbsolutePoint;
	}
	return FVector2D();
}

void UCfHUDWidget::RegisterTargetWidget(UCfMarkingComponent* InTarget)
{
	if(TargetWidgetClass == nullptr || InTarget == nullptr)
		return;

	LockingTarget = InTarget;
	TargetMarkingWidget = CreateWidget< UUserWidget >( GetWorld(), TargetWidgetClass );
	TargetMarkingWidget->AddToViewport(static_cast<int32>(ECfZOrder::Marker));
}

void UCfHUDWidget::UnregisterTargetWidget()
{
	if(TargetWidgetClass == nullptr)
		return;

	if(TargetMarkingWidget)
	{
		TargetMarkingWidget->RemoveFromParent();
		TargetMarkingWidget = nullptr;
	}
	LockingTarget = nullptr;
}

void UCfHUDWidget::ToggleTargetWidget(UCfMarkingComponent* InTarget)
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
