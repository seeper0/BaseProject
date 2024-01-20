// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CfHUD.generated.h"

/**
 * 
 */
UCLASS()
class CFSKILLSYSTEM_API ACfHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void DrawHUD() override;
	
private:
	static FString PrintfImpl(const TCHAR* Fmt, ...);
	void DrawInfo(UWorld* World, const FVector& Location, const FColor Color, const float Scale, const FString& Text);

public:
	template <typename FmtType, typename... Types>
	void DrawActorInfo(const AActor* Actor, const FColor Color, const float Scale, const FmtType& Fmt, Types... Args)
	{
		const FString Text = PrintfImpl((const TCHAR*)Fmt, Args...);
		DrawInfo(Actor->GetWorld(), Actor->GetActorLocation(), Color, Scale, Text);
	}

private:
	void DrawPlayerInfo();
};
