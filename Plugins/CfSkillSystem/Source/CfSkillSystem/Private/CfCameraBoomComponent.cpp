// Fill out your copyright notice in the Description page of Project Settings.


#include "CfCameraBoomComponent.h"
#include "GameFramework/Character.h"
#include "HUD/CfHUD.h"
#include "HUD/CfOverlayLockOnComponent.h"
#include "CfLogger.h"

void UCfCameraBoomComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(const UCfOverlayLockOnComponent* Target = GetLockingComponent())
	{
		if(APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			FVector WorldAimLocation;
			FVector WorldAimDirection;
			FVector WorldCenterLocation;
			if(ACfHUD::GetAimWorldTransform(GetWorld(), WorldAimLocation, WorldAimDirection, WorldCenterLocation))
			{
				const FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
				const FRotator CameraRotation = PC->PlayerCameraManager->GetCameraRotation();
				const FVector AimDirection = (WorldAimLocation - CameraLocation).GetSafeNormal();
				const FVector TargetDirection = (Target->GetComponentLocation() - CameraLocation).GetSafeNormal();
				const FQuat AimToTargetRotation = FQuat::FindBetweenNormals(AimDirection, TargetDirection);
				FRotator TargetRotation = (AimToTargetRotation * CameraRotation.Quaternion()).Rotator();
				TargetRotation.Roll = 0.0f;

				FRotator CurrentRotation = PC->PlayerCameraManager->GetCameraRotation();
				CurrentRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 7.5f);
				PC->SetControlRotation(CurrentRotation);
			}
		}
	}
}

void UCfCameraBoomComponent::ToggleLockOn()
{
	UCfOverlayLockOnComponent* Target = FindLockingTarget();
	ACfHUD::ToggleTargetWidget(GetWorld(), Target);
}

UCfOverlayLockOnComponent* UCfCameraBoomComponent::GetLockingComponent() const
{
	return ACfHUD::GetLockingTarget(GetWorld());
}

UCfOverlayLockOnComponent* UCfCameraBoomComponent::FindLockingTarget() const
{
	FVector WorldAimLocation;
	FVector WorldAimDirection;
	FVector WorldCenterLocation;
	bool bOverlap = false;
	TArray<FOverlapResult> Overlaps;

	if(ACfHUD::GetAimWorldTransform(GetWorld(), WorldAimLocation, WorldAimDirection, WorldCenterLocation))
	{
		FCollisionObjectQueryParams ObjectParams;
		ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
		ObjectParams.AddObjectTypesToQuery(ECC_Vehicle);
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());

		const FVector OwnerLocation = GetOwner()->GetActorLocation();
		const FQuat OwnerRotation = GetOwner()->GetActorRotation().Quaternion();
		
		bOverlap = GetWorld()->OverlapMultiByObjectType(Overlaps, OwnerLocation, OwnerRotation, ObjectParams, FCollisionShape::MakeSphere(LockingDistance), CollisionParams);
	}

	TArray<UCfOverlayLockOnComponent*> AllOverlayLockOnComponents;
	UCfOverlayLockOnComponent* NearLockOnComponent = nullptr;
	if(bOverlap)
	{
		for(const FOverlapResult& Overlap : Overlaps)
		{
			if(ACharacter* Pawn = Cast<ACharacter>(Overlap.GetActor()))
			{
				TArray< UCfOverlayLockOnComponent* > Components;
				Pawn->GetComponents<UCfOverlayLockOnComponent>(Components);
				AllOverlayLockOnComponents.Append(Components);
			}
		}
	}

	float MinDistSq = LockingDistance * LockingDistance;
	UCfOverlayLockOnComponent* NearDistLockOnComponent = nullptr;
	float MinAngle = LockingAngle;
	UCfOverlayLockOnComponent* NearAngleLockOnComponent = nullptr;
	for(UCfOverlayLockOnComponent* LockOn :  AllOverlayLockOnComponents)
	{
		FVector LockOnLocation = LockOn->GetComponentLocation();
		const float DistSq = FVector::DistSquared(WorldAimLocation, LockOnLocation);
		//::DrawDebugSphere(GetWorld(), LockOnLocation, 100.0f, 12, FColor::Red, false, 10.0f);
		if(DistSq < MinDistSq)
		{
			NearDistLockOnComponent = LockOn;
			MinDistSq = DistSq;
		}
			
		const FVector LockOnDirection = (LockOnLocation - WorldAimLocation).GetSafeNormal();
		const float Angle = FMath::Acos( FVector::DotProduct(WorldAimDirection, LockOnDirection) );
		if(Angle < MinAngle)
		{
			NearAngleLockOnComponent = LockOn;
			MinAngle = Angle;
		}
	}
	NearLockOnComponent = NearAngleLockOnComponent ? NearAngleLockOnComponent : NearDistLockOnComponent;

	return NearLockOnComponent;
}
