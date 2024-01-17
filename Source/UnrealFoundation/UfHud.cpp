// Fill out your copyright notice in the Description page of Project Settings.


#include "UfHud.h"
#include "EngineUtils.h"
#include "UfUtil.h"
#include "UfSkillComponent.h"
#include "UnrealFoundationCharacter.h"

void AUfHud::DrawHUD()
{
	Super::DrawHUD();

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

