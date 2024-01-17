// Fill out your copyright notice in the Description page of Project Settings.


#include "UfCheatManager.h"
#include "UfLogger.h"
#include "UfHUD.h"

void UUfCheatManager::ChangeSpeed(int32 ChangeType)
{
	switch(ChangeType)
	{
	case 1: // speed up
		UF_LOG(TEXT("+++"));
		break;
	case 2: // speed down
		UF_LOG(TEXT("---"));
		break;
	}
}

void UUfCheatManager::TogglePlayerInfo()
{
	if(GetWorld())
	{
		if(APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			if(AUfHUD* HUD = Cast<AUfHUD>(PC->GetHUD()))
			{
				HUD->TogglePlayerInfo();
			}
		}
	}
}
