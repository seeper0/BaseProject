// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CfHudDebuggable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCfHudDebuggable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CFCOMMON_API ICfHudDebuggable
{
	GENERATED_BODY()

private:
	static FString PrintfImpl(const TCHAR* Fmt, ...);
	
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	template <typename FmtType, typename... Types>
	void DrawActorInfo(const AActor* Actor, const FColor Color, const float Scale, const FmtType& Fmt, Types... Args)
	{
		const FString Text = PrintfImpl((const TCHAR*)Fmt, Args...);
		DrawInfo(Actor->GetWorld(), Actor->GetActorLocation(), Color, Scale, Text);
	}

private:
	void DrawInfo(UWorld* World, const FVector& Location, const FColor Color, const float Scale, const FString& Text);
};
