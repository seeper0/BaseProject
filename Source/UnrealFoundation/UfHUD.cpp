// Fill out your copyright notice in the Description page of Project Settings.


#include "UfHUD.h"
#include "EngineUtils.h"
#include "UfUtil.h"
#include "UfSkillComponent.h"
#include "UnrealFoundationCharacter.h"

void AUfHUD::DrawHUD()
{
	Super::DrawHUD();

	if(bShowPlayerInfo)
	{
		DrawPlayerInfo();
	}
}

#pragma region PlayerInfo
void AUfHUD::TogglePlayerInfo()
{
	bShowPlayerInfo = !bShowPlayerInfo;
}

void AUfHUD::DrawPlayerInfo()
{
	for (TActorIterator<AUnrealFoundationCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if(AUnrealFoundationCharacter* Char = *ActorItr)
		{
			if(UUfSkillComponent* SkillComponent = Char->GetSkillComponent())
			{
				//FString::Printf()
				DrawActorInfo(Char, FColor::Cyan, 1.0f, TEXT("State : %s"),* FUfUtil::GetEnumString(SkillComponent->GetSkillState()));
			}
		}
	}
}
#pragma endregion

