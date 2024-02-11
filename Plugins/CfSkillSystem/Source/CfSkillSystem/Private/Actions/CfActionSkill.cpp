// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CfActionSkill.h"
#include "GameFramework/Character.h"
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

	if(SkillTable && Owner &&
		SkillTable->Orientation == ECfSkillOrientation::AimOriented)
	{
		bReachedDesiredRotation = false;
	}
	CameraBoomComponent = Owner->GetComponentByClass<UCfCameraBoomComponent>();
}

void UCfActionSkill::OnTick(float DeltaTime)
{
	Super::OnTick(DeltaTime);

	const UCfMarkingComponent* Target = CameraBoomComponent ? CameraBoomComponent->GetLockingComponent() : nullptr;

	if(!bReachedDesiredRotation && Target == nullptr)
	{
		const float CurrentYaw = Owner->GetActorRotation().Yaw;
		const float TargetYaw = Owner->GetBaseAimRotation().Yaw;
		const float NewYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 15.0f);
		Owner->SetActorRotation(FRotator(0, NewYaw, 0));
		if(FMath::IsNearlyEqual(TargetYaw, NewYaw, 0.1f))
		{
			bReachedDesiredRotation = true;
		}
	}
}
