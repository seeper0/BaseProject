// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CfTargetInfoWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"
#include "HUD/CfHUDWidget.h"
#include "Kismet/GameplayStatics.h"

void UCfTargetInfoWidget::Update(AActor* DamagedActor, float TargetInfoHeight, int32 HP, int32 MaxHP)
{
	if(DamagedActor == nullptr)
		return;
	
	FVector Location = DamagedActor->GetActorLocation();
	Location += FVector(0, TargetInfoHeight, 0);

	const TObjectPtr< APlayerController > Pc = UGameplayStatics::GetPlayerController( GetWorld(), 0 );
	if (Pc)
	{
		FVector2D OutScreenPos;
		if(UGameplayStatics::ProjectWorldToScreen( Pc, Location, OutScreenPos ))
		{
			if ( UCanvasPanelSlot* PanelSlot = Cast< UCanvasPanelSlot >( Slot ) )
			{
				FVector2D ParentPoint;
				if(const UCfHUDWidget* HUDWidget = Cast<UCfHUDWidget>(GetParent()))
				{
					ParentPoint = HUDWidget->GetStartPoint();
				}
				PanelSlot->SetPosition( OutScreenPos - ParentPoint );
			}
		}
	}
}
