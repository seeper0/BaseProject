// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CfOverlayInfoComponent.h"
#include "HUD/CfTargetInfoWidget.h"

void UCfOverlayInfoComponent::BeginPlay()
{
	Super::BeginPlay();

	Update(GetOwner(), 1, 1);
}

void UCfOverlayInfoComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bHideDeferred && Progress >= 1.0f)
	{
		bShowWidget = false;
		bHideDeferred = false;
	}
}

void UCfOverlayInfoComponent::ShowWidget(bool bShow)
{
	if(bShowWidget && !bShow)
	{
		bHideDeferred = true;
		HideTime = HideDelayTime;
	}
	else
	{
		Super::ShowWidget(bShow);
	}
}

void UCfOverlayInfoComponent::Update(AActor* DamagedActor, int32 HP, int32 MaxHP)
{
	if(UCfTargetInfoWidget* InfoWidget = Cast<UCfTargetInfoWidget>(GetWidget()))
	{
		InfoWidget->Update(GetOwner(), HP, MaxHP);
	}
}

