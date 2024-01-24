// Fill out your copyright notice in the Description page of Project Settings.


#include "CfActionBase.h"
#include "GameFramework/Character.h"
#include "CfLogger.h"
#include "CfActionComponent.h"
#include "CfActionSkill.h"
#include "CfActionJump.h"
#include "CfActionHit.h"
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
	CF_TODO("여기서 KnockBack, Down, Airborne 등으로 나뉜다.");
	UCfActionHit* ActionHit = nullptr;
	ActionHit = NewObject<UCfActionHit>();
	if(ActionHit)
	{
		ActionHit->InitHit(InOwner, InComponent, DamageEvent);
	}
	return ActionHit;
}

void UCfActionBase::InitAction(ACharacter* InOwner, UCfActionComponent* InComponent, UAnimMontage* InMontage)
{
	Owner = InOwner;
	Component = InComponent;
	Montage = InMontage;
}

FName UCfActionBase::GetActionName() const
{
	return NAME_None;
}

FString UCfActionBase::ToString() const
{
	return TEXT("Not Skill");
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
