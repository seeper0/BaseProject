// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CfOverlayComponent.h"
#include "CfLogger.h"

void UCfOverlayComponent::BeginPlay()
{
	Super::BeginPlay();

	if(UUserWidget* OverlayWidget = GetWidget())
	{
		OverlayWidget->SetVisibility(ESlateVisibility::Hidden);
		OverlayWidget->SetRenderOpacity(0.0f);
	}
}

void UCfOverlayComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(UUserWidget* OverlayWidget = GetWidget())
	{
		if(bShowWidget)
		{
			if(Progress < 1.0f)
			{
				Progress = FMath::Clamp(Progress + DeltaTime * AlphaSpeed, 0.0f, 1.0f);
				//const float Alpha = FMath::InterpEaseInOut(0, 1, Progress, 2.0f);
				OverlayWidget->SetRenderOpacity(Progress);
				//CF_LOG(TEXT("Alpha1 : %f (%f)"), Alpha, Progress);
			}
		}
		else
		{
			HideTime = FMath::Clamp(HideTime - DeltaTime, 0.0f, HideDelayTime);
			if(Progress > 0.0f && HideTime == 0.0f)
			{
				Progress = FMath::Clamp(Progress - DeltaTime * AlphaSpeed, 0.0f, 1.0f);
				//const float Alpha = FMath::InterpEaseInOut(0, 1, Progress, 1.0f);
				OverlayWidget->SetRenderOpacity(Progress);
				//CF_LOG(TEXT("Alpha2 : %f (%f)"), Alpha, Progress);
				if(Progress == 0)
				{
					OverlayWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	}
}

void UCfOverlayComponent::ShowWidget(bool bShow)
{
	if(UUserWidget* OverlayWidget = GetWidget())
	{
		bShowWidget = bShow;
		HideTime = HideDelayTime;

		if(OverlayWidget)
		{
			OverlayWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

