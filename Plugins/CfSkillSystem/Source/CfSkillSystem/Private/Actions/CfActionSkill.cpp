// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CfActionSkill.h"
#include "GameFramework/Character.h"
#include "CfSkillData.h"
#include "HUD/CfOverlayLockOnComponent.h"
#include "CfCameraBoomComponent.h"
#include "CfSkillInputComponent.h"
#include "Actions/CfActionComponent.h"

void UCfActionSkill::InitSkill(ACharacter* InOwner, UCfActionComponent* InComponent, const FCfSkillData* InSkillTable, const FVector& InInputDirection)
{
	Super::InitAction(InOwner, InComponent, InSkillTable->Montage);
	InputComponent = InOwner->GetComponentByClass<UCfSkillInputComponent>();
	SkillTable = InSkillTable;
	InputDirection = InInputDirection;
}

FName UCfActionSkill::GetActionName() const
{
	if(GetSkillTable())
	{
		return GetSkillTable()->RowName;
	}
	return Super::GetActionName();
}

FString UCfActionSkill::ToString() const
{
	if(GetSkillTable())
	{
		return GetSkillTable()->RowName.ToString();
	}
	return Super::ToString();
}

bool UCfActionSkill::CanMoveDuring() const
{
	return (Owner && SkillTable && SkillTable->MobileType == ECfMobileType::Input);
}

bool UCfActionSkill::CanInputAutoRapid() const
{
	return SkillTable && SkillTable->InputType == ECfInputType::AutoRapid;
}

bool UCfActionSkill::HasSkillKey(ECfSkillKey InSkillKey) const
{
	return SkillTable && SkillTable->InputKey == InSkillKey;
}

void UCfActionSkill::OnBegin()
{
	Super::OnBegin();
	
	check(Component);
	check(SkillTable);
	check(Owner);

	if(UCfOverlayLockOnComponent* LockonComponent = UCfCameraBoomComponent::GetLockingComponent(Owner->GetWorld()))
	{
		Target = LockonComponent;
	}
	else
	{
		Target = UCfCameraBoomComponent::FindLockingTarget(Owner,  SkillTable->LockingRange, 180.0f);
	}

	if(SkillTable->Orientation == ECfSkillOrientation::AimOriented)
	{
		bReachedDesiredRotation = false;
	}
	else if(SkillTable->Orientation == ECfSkillOrientation::AutoOriented && Target != nullptr)
	{
		bReachedDesiredRotation = false;
	}
	else
	{
		bReachedDesiredRotation = true;
	}

	CameraBoomComponent = Owner->GetComponentByClass<UCfCameraBoomComponent>();
	ElapsedTime = 0.0f;
	Component->SetChargeLevel(-1);

	// InputDirection
	if(SkillTable->Orientation == ECfSkillOrientation::MoveOriented)
	{
		// ::DrawDebugDirectionalArrow(Owner->GetWorld(),
		// 	Owner->GetActorLocation(),
		// 	Owner->GetActorLocation() + InputDirection * SkillTable->SkillMoveRange,
		// 	10.0f, FColor::Red, false, 10);
		InputDirection = InputComponent->GetInputDirection(InputDirection, SkillTable->SkillDefaultDirection);
		// ::DrawDebugDirectionalArrow(Owner->GetWorld(),
		// 	Owner->GetActorLocation(),
		// 	Owner->GetActorLocation() + InputDirection * SkillTable->SkillMoveRange,
		// 	10.0f, FColor::Blue, false, 10);

		InitMoveSmooth(Owner, InputDirection, SkillTable->SkillMoveRange, SkillTable->SkillMoveTime);
	}
	else
	{
		InitMoveSmooth(Owner);
	}
}

void UCfActionSkill::OnTick(float DeltaTime)
{
	check(Owner);
	check(Owner->GetWorld());
	check(SkillTable);

	Super::OnTick(DeltaTime);
	ElapsedTime += DeltaTime;

	if(Montage == nullptr)
	{
		return;
	}

	if(!bReachedDesiredRotation)
	{
		const float CurrentYaw = Owner->GetActorRotation().Yaw;
		float TargetYaw = Owner->GetBaseAimRotation().Yaw;
		if(SkillTable->Orientation == ECfSkillOrientation::AutoOriented && Target != nullptr)
		{
			TargetYaw = (Target->GetComponentLocation() - Owner->GetActorLocation()).Rotation().Yaw;
		}

		const float NewYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 15.0f);
		Owner->SetActorRotation(FRotator(0, NewYaw, 0));
		if(FMath::IsNearlyEqual(TargetYaw, NewYaw, 0.1f))
		{
			bReachedDesiredRotation = true;
		}
	}

	if(AnimInstance &&
		SkillTable->MobileType == ECfMobileType::AnimCurve)
	{
		const float ForwardValue = AnimInstance->GetCurveValue("ForwardMovement");
		const float UpValue = AnimInstance->GetCurveValue("UpMovement");
		const float YawValue = AnimInstance->GetCurveValue("YawRotation");

		TickMoveSmooth(DeltaTime, Owner, ForwardValue, UpValue, YawValue);
	}

	if(SkillTable->Orientation == ECfSkillOrientation::MoveOriented)
	{
		TickMoveSmooth(DeltaTime, Owner, SkillTable->SkillMoveCurve);
	}

	if(!bReleased &&
		SkillTable->InputType == ECfInputType::Charge)
	{
		if(ElapsedTime > SkillTable->GetChargeInputTime(SkillTable->GetMaxCharge()-1))
		{
			ReleaseButton(SkillTable->InputKey);
		}
	}
}

void UCfActionSkill::OnButtonReleased(const ECfSkillKey InSkillKey)
{
	check(Component);
	check(SkillTable);
	check(AnimInstance);

	if(!bReleased && SkillTable->InputType == ECfInputType::Charge && SkillTable->InputKey == InSkillKey)
	{
		const FName SectionName = TEXT("Action");
		AnimInstance->Montage_JumpToSection(SectionName);

		const int32 ChargeLevel = SkillTable->InputType == ECfInputType::Charge ? SkillTable->GetChargeLevel(ElapsedTime) : -1;
		Component->SetChargeLevel(ChargeLevel);
		bReleased = true;
	}
}
