// Fill out your copyright notice in the Description page of Project Settings.


#include "CfAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "CfCheatManager.h"
#include "CfLogger.h"
#include "CfUtil.h"

void UCfAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	if(APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		CheatManager = Cast<UCfCheatManager>(PC->CheatManager);
	}
}

void UCfAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* Owner = Cast<ACharacter>(TryGetPawnOwner());
	UpdateCharacterMomentum(DeltaSeconds, Owner);
	UpdateCardinalDirection(DeltaSeconds);
	UpdateDebugArrow(DeltaSeconds, Owner);
}

void UCfAnimInstance::UpdateCharacterMomentum(float DeltaSeconds, ACharacter* InOwner)
{
	if(InOwner == nullptr)
		return;

	WasMovingLastUpdate = LocalVelocity2D.IsZero();
	WorldLocation = InOwner->GetActorLocation();
	WorldVelocity = InOwner->GetVelocity();
	WorldRotation = InOwner->GetActorRotation();

	FrameDisplacement = PrevWorldLocation.IsZero() ? 0 : UKismetMathLibrary::VSizeXY(PrevWorldLocation - WorldLocation);
	DisplacementSpeed = UKismetMathLibrary::SafeDivide(FrameDisplacement, DeltaSeconds);
	
	WorldVelocity2D = WorldVelocity * FVector(1.0f, 1.0f, 0.0f);
	LocalVelocity2D = WorldRotation.UnrotateVector(WorldVelocity2D);
	LocalVelocityDirectionAngle = UKismetAnimationLibrary::CalculateDirection(WorldVelocity2D, WorldRotation);
	GroundSpeed = LocalVelocity2D.Length();
	HasVelocity = FMath::IsNearlyZero(GroundSpeed, 0.0f);
	ShouldMove = !FMath::IsNearlyZero(GroundSpeed, 3.0f);
	IsFalling = InOwner->GetMovementComponent()->IsFalling();

	PrevWorldLocation = WorldLocation;
}

void UCfAnimInstance::UpdateCardinalDirection(float DeltaSeconds)
{
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
}

void UCfAnimInstance::UpdateDebugArrow(float DeltaSeconds, ACharacter* InOwner)
{
	if(CheatManager == nullptr || InOwner == nullptr)
		return;

	if(InputArrow == nullptr && CheatManager->IsShowLocomotionInfo())
	{
		TArray<USceneComponent*> ChildComponents;
		InOwner->GetRootComponent()->GetChildrenComponents(false, ChildComponents);
		for(USceneComponent* ChildComponent : ChildComponents)
		{
			if(ChildComponent->GetName() == InputArrowName)
			{
				InputArrow = Cast<UArrowComponent>(ChildComponent);
				InputArrow->SetVisibility(true);
				InputArrow->SetArrowLength(0.0f);
			}
			else if(ChildComponent->GetName() == VelocityArrowName)
			{
				VelocityArrow = Cast<UArrowComponent>(ChildComponent);
				VelocityArrow->SetVisibility(true);
				VelocityArrow->SetArrowLength(0.0f);
			}
		}
	}
	else if(InputArrow && !CheatManager->IsShowLocomotionInfo())
	{
		if(InputArrow)
		{
			InputArrow->SetVisibility(false);
			InputArrow = nullptr;
		}
		if(VelocityArrow)
		{
			VelocityArrow->SetVisibility(false);
			VelocityArrow = nullptr;
		}
	}

	if(InputArrow)
	{
		VelocityArrow->SetWorldRotation(WorldVelocity2D.IsZero() ? InOwner->GetActorRotation() : WorldVelocity2D.Rotation());
		VelocityArrow->SetArrowLength(WorldVelocity2D.Length() * 50.0f / 375.0f);
	}
}
