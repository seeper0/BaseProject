// Fill out your copyright notice in the Description page of Project Settings.


#include "Anims/CfWeaponAnimInstance.h"
#include "Anims/CfAnimInstance.h"
#include "Anims/CfAnimLocomotionData.h"
#include "CfLogger.h"
#include "CfSkillAsset.h"

void UCfWeaponAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	const UDataTable* LocomotionTable = UCfSkillAsset::GetLocomotionTable();
	if(LocomotionTable == nullptr)
		return;

	CF_TODO("데이터 많아지만 문제가 될 부분. 일단 그냥 진행한다.");
	TArray<FName> RowNames = LocomotionTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		const FCfAnimLocomotionData* RowData = LocomotionTable->FindRow<FCfAnimLocomotionData>(RowName, CF_FUNCTION);
		if(RowData->CharacterName == CharacterName && RowData->WeaponType == WeaponType)
		{
			LocomotionData = RowData;
			break;
		}
	}
}

UCfAnimInstance* UCfWeaponAnimInstance::GetMainAnimInstance()
{
	if(USkeletalMeshComponent* MeshComponent = GetOwningComponent())
	{
		if(UCfAnimInstance* MainAnimInstance =  Cast<UCfAnimInstance>(MeshComponent->GetAnimInstance()))
		{
			return MainAnimInstance;
		}
	}
	return nullptr;
}

#define RETURN_SEQUENCE(SequenceName) \
	if(LocomotionData) \
	{ \
		return LocomotionData->SequenceName; \
	} \
	return nullptr;

UAnimSequence* UCfWeaponAnimInstance::GetIdle() const
{
	RETURN_SEQUENCE(Idle);
}

UAnimSequence* UCfWeaponAnimInstance::GetWalk() const
{
	RETURN_SEQUENCE(Walk);
}

UAnimSequence* UCfWeaponAnimInstance::GetWalkFwd() const
{
	RETURN_SEQUENCE(WalkFwd);
}

UAnimSequence* UCfWeaponAnimInstance::GetWalkFwdLeft() const
{
	RETURN_SEQUENCE(WalkFwdLeft)
}

UAnimSequence* UCfWeaponAnimInstance::GetWalkFwdRight() const
{
	RETURN_SEQUENCE(WalkFwdRight)
}

UAnimSequence* UCfWeaponAnimInstance::GetWalkLeft() const
{
	RETURN_SEQUENCE(WalkLeft)
}

UAnimSequence* UCfWeaponAnimInstance::GetWalkRight() const
{
	RETURN_SEQUENCE(WalkRight)
}

UAnimSequence* UCfWeaponAnimInstance::GetWalkBwd() const
{
	RETURN_SEQUENCE(WalkBwd)
}

UAnimSequence* UCfWeaponAnimInstance::GetWalkBwdLeft() const
{
	RETURN_SEQUENCE(WalkBwdLeft)
}

UAnimSequence* UCfWeaponAnimInstance::GetWalkBwdRight() const
{
	RETURN_SEQUENCE(WalkBwdRight)
}

UAnimSequence* UCfWeaponAnimInstance::GetRunFwd() const
{
	RETURN_SEQUENCE(RunFwd)
}

UAnimSequence* UCfWeaponAnimInstance::GetRunFwdLeft() const
{
	RETURN_SEQUENCE(RunFwdLeft)
}

UAnimSequence* UCfWeaponAnimInstance::GetRunFwdRight() const
{
	RETURN_SEQUENCE(RunFwdRight)
}

UAnimSequence* UCfWeaponAnimInstance::GetRunLeft() const
{
	RETURN_SEQUENCE(RunLeft)
}

UAnimSequence* UCfWeaponAnimInstance::GetRunRight() const
{
	RETURN_SEQUENCE(RunRight)
}

UAnimSequence* UCfWeaponAnimInstance::GetRunBwd() const
{
	RETURN_SEQUENCE(RunBwd)
}

UAnimSequence* UCfWeaponAnimInstance::GetRunBwdLeft() const
{
	RETURN_SEQUENCE(RunBwdLeft)
}

UAnimSequence* UCfWeaponAnimInstance::GetRunBwdRight() const
{
	RETURN_SEQUENCE(RunBwdRight)
}

UAnimSequence* UCfWeaponAnimInstance::GetJumpStart() const
{
	RETURN_SEQUENCE(JumpStart)
}

UAnimSequence* UCfWeaponAnimInstance::GetJumpLoop() const
{
	RETURN_SEQUENCE(JumpLoop)
}

UAnimSequence* UCfWeaponAnimInstance::GetJumpEnd() const
{
	RETURN_SEQUENCE(JumpEnd)
}

#undef RETURN_SEQUENCE
