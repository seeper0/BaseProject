// Fill out your copyright notice in the Description page of Project Settings.


#include "SmoothMovable.h"

#include "CfLogger.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Add default functionality here for any ISmoothMovable functions that are not pure virtual.
void ISmoothMovable::InitMoveSmooth(const ACharacter* Owner, const float InDistance, const float InMovingTime)
{
	check(Owner);

	bActorForward = true;
	ElapsedTime = 0.0f;
	Distance = InDistance;
	MovingTime = InMovingTime;

	PrevForwardValue = 0.0f;
	PrevUpValue = 0.0f;
	PrevYawValue = 0.0f;
}

void ISmoothMovable::InitMoveSmooth(const ACharacter* Owner, const FVector& InForward, const float InDistance, const float InMovingTime)
{
	InitMoveSmooth(Owner, InDistance, InMovingTime);

	bActorForward = false;
	Forward = InForward;
}

void ISmoothMovable::TickMoveSmooth(const float DeltaTime, const ACharacter* Owner, const UCurveFloat* Curve)
{
	check(Owner);

	ElapsedTime += DeltaTime;
	float DistProgress = ElapsedTime / MovingTime;
	if(Curve)
	{
		DistProgress = Curve->GetFloatValue(DistProgress);
	}

	const float ForwardValue = FMath::Lerp(0.0f, Distance, DistProgress);
	const float ForwardSpeed = (ForwardValue - PrevForwardValue) / DeltaTime;
	const FVector ForwardDirection = bActorForward ? Owner->GetActorForwardVector() : Forward;
	const FVector MovementDirection = ForwardSpeed * ForwardDirection;
	PrevForwardValue = ForwardValue;

	Owner->GetCharacterMovement()->MoveSmooth(MovementDirection, DeltaTime);
}

void ISmoothMovable::TickMoveSmooth(const float DeltaTime, ACharacter* Owner, const float ForwardValue, const float UpValue, const float YawValue)
{
	const float ForwardSpeed = (ForwardValue - PrevForwardValue) / DeltaTime;
	const float UpSpeed = (UpValue - PrevUpValue) / DeltaTime;
	const float YawSpeed = (YawValue - PrevYawValue) / DeltaTime;

	PrevForwardValue = ForwardValue;
	PrevUpValue = UpValue;
	PrevYawValue = YawValue;

	const FRotator Rotator(0.f, Owner->GetActorRotation().Yaw + YawSpeed * DeltaTime, 0.f);
	Owner->SetActorRotation(Rotator);

	const FVector ForwardDirection = Owner->GetActorForwardVector();
	const FVector MovementDirection = ForwardSpeed * ForwardDirection + FVector(0.f, 0.f, UpSpeed);
	Owner->GetCharacterMovement()->MoveSmooth(MovementDirection, DeltaTime);
}
