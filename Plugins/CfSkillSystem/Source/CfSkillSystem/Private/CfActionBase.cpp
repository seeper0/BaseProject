// Fill out your copyright notice in the Description page of Project Settings.


#include "CfActionBase.h"
#include "GameFramework/Character.h"
#include "CfLogger.h"
#include "CfActionComponent.h"
#include "CfActionSkill.h"
#include "CfActionJump.h"
#include "CfActionHit.h"
#include "CfActionRecovery.h"
#include "CfSkillData.h"

UCfActionBase* UCfActionBase::NewSkill(ACharacter* InOwner, UCfActionComponent* InComponent, const FCfSkillData* InSkillData)
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
		ActionSkill->InitSkill(InOwner, InComponent, InSkillData);
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
	Component = InComponent;
	Montage = InMontage;
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
}

void UCfActionBase::OnButtonReleased(const ECfSkillKey InSkillKey)
{
}

bool UCfActionBase::IsEnd() const
{
	if(Owner && Montage)
	{
		if(Owner->GetMesh() && Owner->GetMesh()->GetAnimInstance())
		{
			return !Owner->GetMesh()->GetAnimInstance()->Montage_IsPlaying(Montage);
		}
	}
	return false;
}

bool UCfActionBase::CanMoveDuring() const
{
	return false;
}

bool UCfActionBase::CanInputDuring() const
{
	return false;
}

bool UCfActionBase::IsSuperArmorActive() const
{
	return false;
}

void UCfActionBase::SetStun(const float InRecoveryTime, const float InStunPlayRate)
{
	RecoveryTime = FMath::Max(InRecoveryTime, 0.0f);
	StunPlayRate = FMath::Max(InStunPlayRate, KINDA_SMALL_NUMBER);
	IsStunned = RecoveryTime > 0.0f;

	if(IsStunned)
	{
		if(const UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance())
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
		if(const UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance())
		{
			if (FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(Montage))
			{
				MontageInstance->SetPlayRate(StunPlayRate);
			}
		}
	}
}
