// Fill out your copyright notice in the Description page of Project Settings.


#include "CfActionHit.h"
#include "CfLogger.h"
#include "CfActionComponent.h"
#include "CfSkillData.h"
#include "CfUtil.h"

void UCfActionHit::InitHit(ACharacter* InOwner, UCfActionComponent* InComponent, const FCfDamageEvent& InDamageEvent)
{
	UAnimMontage* InMontage = nullptr;
	switch(InDamageEvent.SkillData->HitType)
	{
	case ECfHitType::KnockBack:
		InMontage = InComponent->GetKnockBackMontage();
		break;
	case ECfHitType::Down:
		InMontage = InComponent->GetDownMontage();
		break;
	case ECfHitType::Airborne:
		InMontage = InComponent->GetAirborneMontage();
		break;
	default:
		break;
	}

	Super::InitAction(InOwner, InComponent, InMontage);

	DamageEvent = InDamageEvent;
	RemainingTime = DamageEvent.SkillData->HitReactionTime;
}

FName UCfActionHit::GetActionName() const
{
	if(DamageEvent.SkillData)
		DamageEvent.SkillData->RowName;
	return NAME_None;
}

FString UCfActionHit::ToString() const
{
	if(DamageEvent.SkillData)
		return FCfUtil::GetEnumString(DamageEvent.SkillData->HitType);
	return TEXT("Hit");
}

void UCfActionHit::OnBegin()
{
	Super::OnBegin();

	// 모든 히트 모션은 마지막 프레임으로 유지한다.
	if(Owner && Montage && Component)
	{
		if(const UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance())
		{
			if (FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(Montage))
			{
				MontageInstance->bEnableAutoBlendOut = false;
			}
		}
	}
}

void UCfActionHit::OnTick(float DeltaTime)
{
	Super::OnTick(DeltaTime);

	RemainingTime -= DeltaTime;
}

void UCfActionHit::OnEnd()
{
	if(Owner && Montage && Component)
	{
		Owner->StopAnimMontage(Montage);
	}

	Super::OnEnd();
}

bool UCfActionHit::IsEnd() const
{
	if(RemainingTime <= 0.0f)
		return true;

	return false;
}
