// Fill out your copyright notice in the Description page of Project Settings.


#include "CfTargetInfoWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"

void UCfTargetInfoWidget::Update(const FVector& Location, int32 HP, int32 INT32)
{
	const TObjectPtr< APlayerController > Pc = UGameplayStatics::GetPlayerController( GetWorld(), 0 );
	if (Pc)
	{
		FVector2D OutScreenPos;
		if(UGameplayStatics::ProjectWorldToScreen( Pc, Location, OutScreenPos ))
		{
			const float ViewportScale = UWidgetLayoutLibrary::GetViewportScale( this );
			OutScreenPos.X = OutScreenPos.X / ViewportScale;
			OutScreenPos.Y = OutScreenPos.Y / ViewportScale;

			if ( UCanvasPanelSlot* PanelSlot = Cast< UCanvasPanelSlot >( Slot ) )
			{
				PanelSlot->SetPosition( OutScreenPos );
			}
		}
	}
}
