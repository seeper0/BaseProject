// Fill out your copyright notice in the Description page of Project Settings.


#include "UfHUD.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "CfUtil.h"
#include "CfSkillComponent.h"
#include "CfHUDWidget.h"
#include "UnrealFoundationCharacter.h"
#include "UnrealFoundationGameMode.h"

void AUfHUD::BeginPlay()
{
	Super::BeginPlay();

	if(AUnrealFoundationGameMode* Mode = Cast<AUnrealFoundationGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if ( const TObjectPtr< UCfHUDWidget > Widget = CreateWidget< UCfHUDWidget >( GetWorld(), Mode->HudWidgetClass ) )
		{
			Widget->AddToViewport();
		}
	}
}

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
			if(UCfSkillComponent* SkillComponent = Char->GetSkillComponent())
			{
				//FString::Printf()
				DrawActorInfo(Char, FColor::Cyan, 1.0f, TEXT("State : %s"),* FCfUtil::GetEnumString(SkillComponent->GetSkillState()));
			}
		}
	}
}
#pragma endregion


