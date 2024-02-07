// Fill out your copyright notice in the Description page of Project Settings.


#include "CfAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CfLogger.h"
#include "CfUtil.h"

void UCfAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* Owner = Cast<ACharacter>(TryGetPawnOwner());
	if(Owner == nullptr)
		return;

	
	WasMovingLastUpdate = LocalVelocity2D.IsZero();
	WorldLocation = Owner->GetActorLocation();
	WorldVelocity = Owner->GetVelocity();
	WorldRotation = Owner->GetActorRotation();

	FrameDisplacement = PrevWorldLocation.IsZero() ? 0 : UKismetMathLibrary::VSizeXY(PrevWorldLocation - WorldLocation);
	DisplacementSpeed = UKismetMathLibrary::SafeDivide(FrameDisplacement, DeltaSeconds);
	
	WorldVelocity2D = WorldVelocity * FVector(1.0f, 1.0f, 0.0f);
	LocalVelocity2D = WorldRotation.UnrotateVector(WorldVelocity2D);
	LocalVelocityDirectionAngle = UKismetAnimationLibrary::CalculateDirection(WorldVelocity2D, WorldRotation);
	GroundSpeed = LocalVelocity2D.Length();
	HasVelocity = FMath::IsNearlyZero(GroundSpeed, 0.0f);
	ShouldMove = !FMath::IsNearlyZero(GroundSpeed, 3.0f);
	IsFalling = Owner->GetMovementComponent()->IsFalling();

	// Update CardinalDirection
	const float AbsAngle = FMath::Abs(LocalVelocityDirectionAngle);
	if(AbsAngle <= 45)
		CardinalDirection = ECfCardinalDirection::Forward;
	else if(AbsAngle >= 135)
		CardinalDirection = ECfCardinalDirection::Backward;
	else if(LocalVelocityDirectionAngle > 0)
		CardinalDirection = ECfCardinalDirection::Right;
	else
		CardinalDirection = ECfCardinalDirection::Left;
	
	
	PrevWorldLocation = WorldLocation;
}
