// Fill out your copyright notice in the Description page of Project Settings.


#include "CfCheatManager.h"
#include "CfHUD.h"

void UCfCheatManager::TogglePlayerInfo()
{
	if(GetWorld())
	{
		if(APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			if(ACfHUD* HUD = Cast<ACfHUD>(PC->GetHUD()))
			{
				HUD->TogglePlayerInfo();
			}
		}
	}
}
