// Fill out your copyright notice in the Description page of Project Settings.


#include "CfCameraBoomComponent.h"
#include "GameFramework/Character.h"
#include "HUD/CfHUD.h"
#include "HUD/CfOverlayLockOnComponent.h"
#include "CfLogger.h"
#include "CfUtil.h"

void UCfCameraBoomComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(const UCfOverlayLockOnComponent* Target = GetLockingComponent(GetWorld()))
	{
		if(APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			FVector WorldAimLocation;
			FVector WorldAimDirection;
			FVector WorldCenterLocation;
			if(ACfHUD::GetAimWorldTransform(GetWorld(), WorldAimLocation, WorldAimDirection, WorldCenterLocation))
			{
				// 카메라를 타겟 방향으로 회전
				const FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
				const FRotator CameraRotation = PC->PlayerCameraManager->GetCameraRotation();
				const FVector AimDirection = (WorldAimLocation - CameraLocation).GetSafeNormal();
				const FVector TargetDirection = (Target->GetComponentLocation() - CameraLocation).GetSafeNormal();
				const FQuat AimToTargetRotation = FQuat::FindBetweenNormals(AimDirection, TargetDirection);
				FRotator TargetRotation = (AimToTargetRotation * CameraRotation.Quaternion()).Rotator();
				TargetRotation.Roll = 0.0f;

				// 너무 아래를 바라보지 않게 보정
				const FVector ActorDirection = (FCfUtil::GetCapsuleBottomLocation(Target->GetOwner()) - FCfUtil::GetCapsuleBottomLocation(GetOwner())).GetSafeNormal();
				const double BasePitch = FMath::RadiansToDegrees(atan2(ActorDirection.Z, FVector(ActorDirection.X, ActorDirection.Y, 0.0f).Size())) - 30.0;
				TargetRotation.Pitch = FMath::Max(TargetRotation.Pitch, BasePitch);

				// 적용
				FRotator CurrentRotation = PC->PlayerCameraManager->GetCameraRotation();
				CurrentRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 7.5f);
				PC->SetControlRotation(CurrentRotation);
			}
		}
	}
}

void UCfCameraBoomComponent::ToggleLockOn()
{
	UCfOverlayLockOnComponent* Target = FindLockingTarget(GetOwner(), LockingDistance, LockingAngle);
	ACfHUD::ToggleTargetWidget(GetWorld(), Target);
}

UCfOverlayLockOnComponent* UCfCameraBoomComponent::GetLockingComponent(UWorld* World)
{
	check(World);
	return ACfHUD::GetLockingTarget(World);
}

UCfOverlayLockOnComponent* UCfCameraBoomComponent::FindLockingTarget(AActor* Owner, float LockingDistance, float LockingAngle)
{
	FVector WorldAimLocation;
	FVector WorldAimDirection;
	FVector WorldCenterLocation;
	bool bOverlap = false;
	TArray<FOverlapResult> Overlaps;

	if(ACfHUD::GetAimWorldTransform(Owner->GetWorld(), WorldAimLocation, WorldAimDirection, WorldCenterLocation))
	{
		FCollisionObjectQueryParams ObjectParams;
		ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
		ObjectParams.AddObjectTypesToQuery(ECC_Vehicle);
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(Owner);

		const FVector OwnerLocation = Owner->GetActorLocation();
		const FQuat OwnerRotation = Owner->GetActorRotation().Quaternion();
		
		bOverlap = Owner->GetWorld()->OverlapMultiByObjectType(Overlaps, OwnerLocation, OwnerRotation, ObjectParams, FCollisionShape::MakeSphere(LockingDistance), CollisionParams);
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
