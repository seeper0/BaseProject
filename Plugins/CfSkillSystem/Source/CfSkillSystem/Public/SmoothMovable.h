// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SmoothMovable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USmoothMovable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CFSKILLSYSTEM_API ISmoothMovable
{
	GENERATED_BODY()

protected:
	void InitMoveSmooth(const ACharacter* Owner, const float InDistance=0.0f, const float InMovingTime=0.0f);
	void InitMoveSmooth(const ACharacter* Owner, const FVector& InForward, const float InDistance, const float InMovingTime);
	void TickMoveSmooth(const float DeltaTime, const ACharacter* Owner, const UCurveFloat* Curve);
	void TickMoveSmooth(const float DeltaTime, ACharacter* Owner, const float ForwardValue, const float UpValue, const float YawValue);

public:
	bool bActorForward = true;
	FVector Forward;
	float ElapsedTime = 0.0f;
	float Distance = 0.0f;
	float MovingTime = 0.0f;

	float PrevForwardValue = 0.0f;
	float PrevUpValue = 0.0f;
	float PrevYawValue = 0.0f;
};
