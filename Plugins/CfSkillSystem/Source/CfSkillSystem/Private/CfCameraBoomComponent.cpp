// Fill out your copyright notice in the Description page of Project Settings.


#include "CfCameraBoomComponent.h"
#include "HUD/CfHUD.h"
#include "CfMarkingComponent.h"
#include "CfLogger.h"

void UCfCameraBoomComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(const UCfMarkingComponent* Target = GetLockingComponent())
	{
		if(APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			FVector WorldAimLocation;
			FVector WorldAimDirection;
			if(ACfHUD::GetAimWorldTransform(GetWorld(), WorldAimLocation, WorldAimDirection))
			{
				CF_TODO("스크린좌표부터 각도까지 정확하지 않다. 대충 코딩했으니 하나씩 맞춰봐야함")
				//::DrawDebugDirectionalArrow(GetWorld(), WorldAimLocation, WorldAimLocation+WorldAimLocation * 10, 1, FColor::Red, false, 10.0f);
				//::DrawDebugSphere(GetWorld(), WorldAimLocation, 10, 12, FColor::Red, false, 10.0f);
				//::DrawDebugSphere(GetWorld(), Target->GetComponentLocation(), 10, 12, FColor::Blue, false, 10.0f);
				FRotator CurrentRot = PC->GetDesiredRotation();
				const FRotator AimRot = (Target->GetComponentLocation() - WorldAimLocation).Rotation();
				FRotator CamRot = (Target->GetComponentLocation() - GetUnfixedCameraPosition()).Rotation();
				// float NewPitch = AimRot.Pitch - CamRot.Pitch;
				// CamRot.Pitch = CamRot.Pitch + NewPitch;

				//CF_LOG(TEXT("%f / %f"), AimRot.Pitch, TargetRot.Pitch);
				//TargetRot.Pitch = AimRot.Pitch;
				//TargetRot.Pitch -= 10;
				//::DrawDebugLine(GetWorld(), WorldAimLocation, Target->GetComponentLocation(), FColor::Red);
				//::DrawDebugLine(GetWorld(), GetUnfixedCameraPosition(), Target->GetComponentLocation(), FColor::Blue);
				CurrentRot = FMath::RInterpTo(CurrentRot, CamRot, DeltaTime, 7.5f);
				PC->SetControlRotation(CurrentRot);
			}
		}
	}
}

void UCfCameraBoomComponent::ToggleLockOn()
{
	UCfMarkingComponent* LockingTarget = FindLockingTarget();
	ACfHUD::ToggleTargetWidget(GetWorld(), LockingTarget);
}

UCfMarkingComponent* UCfCameraBoomComponent::GetLockingComponent() const
{
	return ACfHUD::GetLockingTarget(GetWorld());
}

UCfMarkingComponent* UCfCameraBoomComponent::FindLockingTarget() const
{
	FVector WorldAimLocation;
	FVector WorldAimDirection;
	bool bOverlap = false;
	TArray<FOverlapResult> Overlaps;

	if(ACfHUD::GetAimWorldTransform(GetWorld(), WorldAimLocation, WorldAimDirection))
	{
		FCollisionObjectQueryParams ObjectParams;
		ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
		ObjectParams.AddObjectTypesToQuery(ECC_Vehicle);
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());

		const FVector OwnerLocation = GetOwner()->GetActorLocation();
		const FQuat OwnerRotation = GetOwner()->GetActorRotation().Quaternion();
		
		bOverlap = GetWorld()->OverlapMultiByObjectType(Overlaps, OwnerLocation, OwnerRotation, ObjectParams, FCollisionShape::MakeSphere(LockingDistance), CollisionParams);
	}

	UCfMarkingComponent* NearMarkingComponent = nullptr;
	if(bOverlap)
	{
		float MinDistSq = LockingDistance * LockingDistance;
		UCfMarkingComponent* NearDistMarkingComponent = nullptr;
		float MinAngle = LockingAngle;
		UCfMarkingComponent* NearAngleMarkingComponent = nullptr;
		for(const FOverlapResult& Overlap : Overlaps)
		{
			if(UCfMarkingComponent* MarkingComponent = Cast<UCfMarkingComponent>(Overlap.GetComponent()))
			{
				if(MarkingComponent->ComponentHasTag(UCfMarkingComponent::TAG_Locking))
				{
					FVector MarkingLocation = MarkingComponent->GetComponentLocation();
					const float DistSq = FVector::DistSquared(WorldAimLocation, MarkingLocation);
					//::DrawDebugSphere(GetWorld(), MarkingLocation, 100.0f, 12, FColor::Red, false, 10.0f);
					if(DistSq < MinDistSq)
					{
						NearDistMarkingComponent = MarkingComponent;
						MinDistSq = DistSq;
					}
					
					const FVector MarkingDirection = (MarkingLocation - WorldAimLocation).GetSafeNormal();
					const float Angle = FMath::Acos( FVector::DotProduct(WorldAimDirection, MarkingDirection) );
					if(Angle < MinAngle)
					{
						NearAngleMarkingComponent = MarkingComponent;
						MinAngle = Angle;
					}
				}
			}
		}
		NearMarkingComponent = NearAngleMarkingComponent ? NearAngleMarkingComponent : NearDistMarkingComponent;
	}

	return NearMarkingComponent;
}
