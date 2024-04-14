// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CfActionHit.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actions/CfActionComponent.h"
#include "CfSkillData.h"
#include "CfUtil.h"
#include "CfLogger.h"

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

	MovementComponent = Owner->GetCharacterMovement();
	DamageEvent = InDamageEvent;
	ElapsedTime = 0.0f;

	InitKnockBack(InDamageEvent);
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
	//Super::OnBegin();
	// 모든 히트 모션은 마지막 프레임으로 유지한다.
	if(Montage && AnimInstance && Component)
	{
		AnimInstance->Montage_PlayWithBlendIn(Montage, FAlphaBlendArgs(0.1f * DamageEvent.SkillData->HitStunPlayRate));
		if (FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(Montage))
		{
			MontageInstance->bEnableAutoBlendOut = false;
			SetStun(DamageEvent.SkillData->HitStunDuration, DamageEvent.SkillData->HitStunPlayRate);

			if(UCfActionComponent* CauserComponent = DamageEvent.DamageCauser->GetComponentByClass<UCfActionComponent>())
			{
				CauserComponent->SetStun(DamageEvent.SkillData->HitStopDuration, DamageEvent.SkillData->HitStopPlayRate);
			}
		}
	}
}

void UCfActionHit::OnTick(float DeltaTime)
{
	Super::OnTick(DeltaTime);

	if(DamageEvent.SkillData == nullptr)
		return;

	ElapsedTime += DeltaTime;
	TickKnockBack(GetStunPlayRate() * DeltaTime);
}

void UCfActionHit::OnEnd()
{
	if(Owner && Montage && Component)
	{
		Owner->StopAnimMontage(Montage);

		if(DamageEvent.SkillData)
		{
			switch(DamageEvent.SkillData->HitType)
			{
			case ECfHitType::Down:
			case ECfHitType::Airborne:
				Component->ReserveAction({true});
				break;
			default:
				break;
			}
		}
	}

	Super::OnEnd();
}

bool UCfActionHit::IsEnd() const
{
	if(Owner == nullptr || MovementComponent == nullptr)
		return true;

	if(DamageEvent.SkillData == nullptr)
		return true;

	if(ElapsedTime >= DamageEvent.SkillData->HitReactionTime)
		return true;

	return false;
}

bool UCfActionHit::IsSuperArmorActive() const
{
	switch (DamageEvent.SkillData->HitType )
	{
	case ECfHitType::Down:
	case ECfHitType::Airborne:
		return true;
	default:
		return Super::IsSuperArmorActive();
	}
}

void UCfActionHit::InitKnockBack(const FCfDamageEvent& InDamageEvent)
{
	FVector StartLocation = Owner->GetActorLocation();
	FVector CauserLocation = InDamageEvent.DamageCauser->GetActorLocation();
	StartLocation.Z = 0;
	CauserLocation.Z = 0;

	FVector Direction;
	switch (DamageEvent.SkillData->HitDirection)
	{
	case ECfHitDirection::Backward:
		Direction = InDamageEvent.DamageCauser->GetActorForwardVector();
		break;
	case ECfHitDirection::Radial:
	default:
		Direction = (StartLocation - CauserLocation).GetSafeNormal();
		break;
	}

	Owner->SetActorRotation(Direction.Rotation() + FRotator(0, 180, 0));
	InitMoveSmooth(Owner, Direction, DamageEvent.SkillData->KnockBackDistance, DamageEvent.SkillData->KnockBackDistanceTime);
}

void UCfActionHit::TickKnockBack(float DeltaTime)
{
	TickMoveSmooth(DeltaTime, Owner, DamageEvent.SkillData->KnockBackCurve);
}
