// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CfActionBase.h"
#include "GameFramework/Character.h"
#include "CfLogger.h"
#include "Actions/CfActionComponent.h"
#include "Actions/CfActionSkill.h"
#include "Actions/CfActionJump.h"
#include "Actions/CfActionHit.h"
#include "Actions/CfActionRecovery.h"
#include "CfSkillData.h"

UCfActionBase* UCfActionBase::NewSkill(ACharacter* InOwner, UCfActionComponent* InComponent, const FCfSkillData* InSkillData, const FVector& InputDirection)
{
	UCfActionSkill* ActionSkill = nullptr;
	switch (InSkillData->InputKey)
	{
	case ECfSkillKey::Jump:
		ActionSkill = NewObject<UCfActionJump>();
		break;
	default:
		ActionSkill = NewObject<UCfActionSkill>();
		break;
	}

	if(ActionSkill)
	{
		ActionSkill->InitSkill(InOwner, InComponent, InSkillData, InputDirection);
	}
	return ActionSkill;
}

UCfActionBase* UCfActionBase::NewHitReaction(ACharacter* InOwner, UCfActionComponent* InComponent, const FCfDamageEvent& DamageEvent)
{
	UCfActionHit* ActionHit = nullptr;
	ActionHit = NewObject<UCfActionHit>();
	if(ActionHit)
	{
		ActionHit->InitHit(InOwner, InComponent, DamageEvent);
	}
	return ActionHit;
}

UCfActionBase* UCfActionBase::NewRecover(ACharacter* InOwner, UCfActionComponent* InComponent)
{
	UCfActionRecovery* ActionRecover = nullptr;
	ActionRecover = NewObject<UCfActionRecovery>();
	if(ActionRecover)
	{
		ActionRecover->InitRecover(InOwner, InComponent);
	}
	return ActionRecover;
}

void UCfActionBase::InitAction(ACharacter* InOwner, UCfActionComponent* InComponent, UAnimMontage* InMontage)
{
	Owner = InOwner;
	Movement = Owner ? Owner->GetCharacterMovement() : nullptr;
	Component = InComponent;
	Montage = InMontage;
	Mesh = Owner ? Owner->GetMesh() : nullptr;
	AnimInstance = Mesh ? Mesh->GetAnimInstance() : nullptr;
	ElapsedRecoveryTime = 0.0f;
}

FName UCfActionBase::GetActionName() const
{
	return NAME_None;
}

FString UCfActionBase::ToString() const
{
	return TEXT("Not Skill");
}

void UCfActionBase::Begin()
{
	OnBegin();
}

void UCfActionBase::Tick(float DeltaTime)
{
	OnTick(DeltaTime);
}

void UCfActionBase::End()
{
	OnEnd();
}

void UCfActionBase::ReleaseButton(const ECfSkillKey InSkillKey)
{
	OnButtonReleased(InSkillKey);
}

void UCfActionBase::OnBegin()
{
	//CF_LOG(TEXT("BEGIN"));
	if(Owner && Montage && Component)
	{
		Owner->PlayAnimMontage(Montage);
	}
}

void UCfActionBase::OnTick(float DeltaTime)
{
	TickAnimStun(DeltaTime);
}

void UCfActionBase::OnEnd()
{
	if(AnimInstance)
	{
		AnimInstance->Montage_Stop(0.1f);
	}
}

void UCfActionBase::OnButtonReleased(const ECfSkillKey InSkillKey)
{
}

bool UCfActionBase::IsEnd() const
{
	if(AnimInstance)
	{
		return !AnimInstance->Montage_IsPlaying(Montage);
	}
	return false;
}

bool UCfActionBase::CanMoveDuring() const
{
	return false;
}

bool UCfActionBase::CanInputAutoRapid() const
{
	return false;
}

bool UCfActionBase::CanInputRelease() const
{
	return false;
}

bool UCfActionBase::IsSuperArmorActive() const
{
	return false;
}

bool UCfActionBase::HasSkillKey(ECfSkillKey InSkillKey) const
{
	return false;
}

FName UCfActionBase::GetCurrentSection() const
{
	if(AnimInstance)
	{
		if(AnimInstance->Montage_IsPlaying(Montage))
		{
			return AnimInstance->Montage_GetCurrentSection();
		}
	}
	return NAME_None;
}

bool UCfActionBase::HasSection(FName InSection) const
{
	return GetCurrentSection() == InSection; 
}

void UCfActionBase::SetStun(const float InRecoveryTime, const float InStunPlayRate)
{
	RecoveryTime = FMath::Max(InRecoveryTime, 0.0f);
	StunPlayRate = FMath::Max(InStunPlayRate, KINDA_SMALL_NUMBER);
	IsStunned = RecoveryTime > 0.0f;

	if(IsStunned)
	{
		if(AnimInstance)
		{
			if (FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(Montage))
			{
				MontageInstance->SetPlayRate(StunPlayRate);
			}
		}
	}
	else
	{
		StunPlayRate = 1.0f;
	}
}

void UCfActionBase::TickAnimStun(float DeltaTime)
{
	if(!IsStunned)
		return;
	 
	ElapsedRecoveryTime += DeltaTime;
	if(ElapsedRecoveryTime > RecoveryTime)
	{
		IsStunned = false;
		StunPlayRate = 1.0f;
		if(AnimInstance)
		{
			if (FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(Montage))
			{
				MontageInstance->SetPlayRate(StunPlayRate);
			}
		}
	}
}
