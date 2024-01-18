// Fill out your copyright notice in the Description page of Project Settings.


#include "UfActionBase.h"
#include "GameFramework/Character.h"
#include "UfLogger.h"
#include "UfSkillComponent.h"
#include "UfSkillData.h"
#include "GameFramework/PawnMovementComponent.h"

void UUfActionBase::InitAction(ACharacter* InOwner, UUfSkillComponent* InComponent, UAnimMontage* InMontage, const FUfSkillData* InSkillTable)
{
	Owner = InOwner;
	Component = InComponent;
	Montage = InMontage;
	SkillTable = InSkillTable;
}

FString UUfActionBase::ToString() const
{
	if(GetSkillTable())
	{
		return GetSkillTable()->RowName.ToString();
	}
	return TEXT("Not Skill");
}

void UUfActionBase::OnBegin()
{
	//UF_LOG(TEXT("BEGIN"));
	if(Owner && Montage && Component)
	{
		Owner->PlayAnimMontage(Montage);
	}
	else if(SkillTable && SkillTable->Key == EUfSkillKey::Jump)
	{
		Owner->Jump();
	}
}

void UUfActionBase::OnTick()
{
}

void UUfActionBase::OnMontageEnd()
{
	//Montage = nullptr;
}

void UUfActionBase::OnEnd()
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
	
	//UF_LOG(TEXT("END"));
	// if(Owner && Montage)
	// {
	// 	if(Owner->GetMesh() && Owner->GetMesh()->GetAnimInstance())
	// 	{
	// 		Owner->GetMesh()->GetAnimInstance()->Montage_Stop(0.0f, Montage); // 강제로 멈추고 다른 몽타주 실행하면 튈 수 있다.
	// 	}
	// }
}

void UUfActionBase::OnButtonReleased()
{
	if(SkillTable && SkillTable->Key == EUfSkillKey::Jump)
	{
		Owner->StopJumping();
	}
}

bool UUfActionBase::IsEnd() const
{
	//return Montage == nullptr;
	
	if(Owner && Montage)
	{
		if(Owner->GetMesh() && Owner->GetMesh()->GetAnimInstance())
		{
			return !Owner->GetMesh()->GetAnimInstance()->Montage_IsPlaying(Montage);
		}
	}
	else if(Owner && SkillTable && SkillTable->Key == EUfSkillKey::Jump)
	{
		return !Owner->GetMovementComponent()->IsFalling();
	}

	return false;
}

bool UUfActionBase::CanMove() const
{
	return (Owner && SkillTable && SkillTable->MobileType == EUfMobileType::Input);
}
