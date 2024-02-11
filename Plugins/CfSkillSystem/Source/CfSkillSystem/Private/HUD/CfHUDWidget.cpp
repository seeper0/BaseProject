// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CfHUDWidget.h"
#include "CfSkill.h"
#include "HUD/CfCrossHairWidget.h"

void UCfHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

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
		return CrossHair->GetCachedGeometry().LocalToAbsolute(FVector2D::ZeroVector);
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
