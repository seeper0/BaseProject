// Fill out your copyright notice in the Description page of Project Settings.


#include "CfActionBase.h"
#include "GameFramework/Character.h"
#include "CfLogger.h"
#include "CfSkillComponent.h"
#include "CfSkillData.h"
#include "CfActionJump.h"

UCfActionBase* UCfActionBase::NewSkill(ACharacter* InOwner, UCfSkillComponent* InComponent, const FCfSkillData* InSkillData)
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

void UCfActionBase::InitAction(ACharacter* InOwner, UCfSkillComponent* InComponent, UAnimMontage* InMontage)
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

void UCfActionBase::OnTick()
{
}

void UCfActionBase::OnMontageEnd()
{
	//Montage = nullptr;
}

void UCfActionBase::OnEnd()
{
	// 액션이 종료되는 세가지 조건
	// 1. 몽타주 종료 : OnMontageEnd
	// 2. Tick 종료 : OnEnd
	// 3. 새로운 스킬 시작시 삭제 :
	// 모두 OnEnd를 마지막으로 호출되게 수정해야한다.
	// OnMontageEnd 가 호출되어도 잔여타임이 있다면 OnEnd가 호출되지 않는다.
	// 스킬이 강제 중단된다면 OnEnd가 호출되고 OnMontageEnd가 호출될 수 있다.
	// 1. 다른거 시작되기 직전
	// 2. 자동으로 끝날때
	
	//CF_LOG(TEXT("END"));
	// if(Owner && Montage)
	// {
	// 	if(Owner->GetMesh() && Owner->GetMesh()->GetAnimInstance())
	// 	{
	// 		Owner->GetMesh()->GetAnimInstance()->Montage_Stop(0.0f, Montage); // 강제로 멈추고 다른 몽타주 실행하면 튈 수 있다.
	// 	}
	// }
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
