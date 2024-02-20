// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CfTargetInfoWidget.h"
#include "Components/ProgressBar.h"

void UCfTargetInfoWidget::Update(AActor* DamagedActor, int32 HP, int32 MaxHP)
{
	if(DamagedActor == nullptr)
		return;

	if(HpBar)
	{
		const float Percent = static_cast<float>(HP) / static_cast<float>(MaxHP);
		HpBar->SetPercent(Percent);
	}
}
