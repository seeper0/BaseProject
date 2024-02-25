// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CfActionSkill.h"
#include "GameFramework/Character.h"
#include "HUD/CfOverlayLockOnComponent.h"
#include "CfCameraBoomComponent.h"
#include "CfSkillData.h"

void UCfActionSkill::InitSkill(ACharacter* InOwner, UCfActionComponent* InComponent, const FCfSkillData* InSkillTable)
{
	Super::InitAction(InOwner, InComponent, InSkillTable->Montage);
	SkillTable = InSkillTable;
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

bool UCfActionSkill::CanInputDuring() const
{
	if(SkillTable)
	{
		switch (SkillTable->InputType)
		{
		case ECfInputType::AutoRapid:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void UCfActionSkill::OnBegin()
{
	Super::OnBegin();

	if(SkillTable == nullptr || Owner == nullptr)
		return;

	Target = UCfCameraBoomComponent::FindLockingTarget(Owner,  SkillTable->Range, 180.0f);
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
}

void UCfActionSkill::OnTick(float DeltaTime)
{
	check(Owner);
	check(Owner->GetWorld());

	Super::OnTick(DeltaTime);

	const UCfOverlayLockOnComponent* LockonComponent = UCfCameraBoomComponent::GetLockingComponent(Owner->GetWorld());

	if(!bReachedDesiredRotation && LockonComponent == nullptr)
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
}
