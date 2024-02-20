// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CfOverlayComponent.h"

void UCfOverlayComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetWidget())
	{
		GetWidget()->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCfOverlayComponent::ShowWidget(bool bShow)
{
	if(UUserWidget* OverlayWidget = GetWidget())
	{
		if(bShow && OverlayWidget->GetVisibility() == ESlateVisibility::Hidden)
		{
			OverlayWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else if(!bShow && OverlayWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			OverlayWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

