// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CfHUD.h"

#include "CfCheatManager.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "CfUtil.h"
#include "Actions/CfActionComponent.h"

void ACfHUD::DrawHUD()
{
	Super::DrawHUD();
	
	if(APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if(UCfCheatManager* CM = Cast<UCfCheatManager>(PC->CheatManager))
		{
			if(CM->IsShowPlayerInfo())
			{
				DrawPlayerInfo();
			}
		}
	}
}

FString ACfHUD::PrintfImpl(const TCHAR* Fmt, ...)
{
	constexpr int32 STARTING_BUFFER_SIZE = 512;
	int32 BufferSize = STARTING_BUFFER_SIZE;
	TCHAR StartingBuffer[STARTING_BUFFER_SIZE];
	TCHAR* Buffer = StartingBuffer;
	int32 Result = -1;

	// First try to print to a stack allocated location 
	GET_VARARGS_RESULT( Buffer, BufferSize, BufferSize-1, Fmt, Fmt, Result );

	// If that fails, start allocating regular memory
	if( Result == -1 )
	{
		Buffer = nullptr;
		while(Result == -1)
		{
			BufferSize *= 2;
			Buffer = (TCHAR*) FMemory::Realloc( Buffer, BufferSize * sizeof(TCHAR) );
			GET_VARARGS_RESULT( Buffer, BufferSize, BufferSize-1, Fmt, Fmt, Result );
		};
	}

	Buffer[Result] = TEXT('\0');

	FString ResultString(Buffer);

	if( BufferSize != STARTING_BUFFER_SIZE )
	{
		FMemory::Free( Buffer );
	}

	return ResultString;
}

void ACfHUD::DrawInfo(UWorld* World, const FVector& Location, const FColor Color, const float Scale,
	const FString& Text)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
	if (PlayerController == nullptr)
		return;

	int32 ScreenWidth = 0;
	int32 ScreenHeight = 0;
	FVector2D ScreenLocation;
	PlayerController->GetViewportSize(ScreenWidth, ScreenHeight);
	PlayerController->ProjectWorldLocationToScreen(Location, ScreenLocation);

	int32 ScreenX = static_cast<int32>(ScreenLocation.X) + 50.0f;
	int32 ScreenY = static_cast<int32>(ScreenLocation.Y) - 150.0f;
	const int32 StartX = ScreenX;
	const int32 StartY = ScreenY;
	float BoxWidth = 0;
	float BoxHeight = 0;

	TArray<FString> StringArray;
	Text.ParseIntoArrayLines(StringArray);
	for(const FString& Msg : StringArray)
	{
		float MsgWidth = 0, MsgHeight = 0;
		DrawText(Msg, Color, ScreenX, ScreenY, nullptr, Scale);
		GetTextSize(Msg, MsgWidth, MsgHeight, nullptr, Scale);

		BoxWidth = FMath::Max(BoxWidth, MsgWidth);
		BoxHeight += MsgHeight + 2;
		ScreenY += MsgHeight + 2;
	}

	const FColor ComplementaryColor = FColor(255 - Color.R, 255 - Color.G, 255 - Color.B, 50);
	DrawRect(ComplementaryColor, StartX-5, StartY-5, BoxWidth+10, BoxHeight+10);
}

void ACfHUD::DrawPlayerInfo()
{
	for (TActorIterator<ACharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if(ACharacter* Char = *ActorItr)
		{
			if(const UCfActionComponent* SkillComponent = UCfActionComponent::GetActionComponent(Char))
			{
				DrawActorInfo(Char, FColor::Cyan, 1.0f, TEXT("State : %s"),* FCfUtil::GetEnumString(SkillComponent->GetSkillState()));
			}
		}
	}
}


