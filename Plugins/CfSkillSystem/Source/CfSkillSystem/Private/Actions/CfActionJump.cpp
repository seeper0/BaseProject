// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CfActionJump.h"
#include "GameFramework/Character.h"
#include "CfSkillData.h"
#include "GameFramework/PawnMovementComponent.h"

void UCfActionJump::OnBegin()
{
	if(SkillTable && SkillTable->InputKey == ECfSkillKey::Jump)
	{
		Owner->Jump();
	}
}

void UCfActionJump::OnEnd()
{
	Super::OnEnd();
}

void UCfActionJump::OnButtonReleased(const ECfSkillKey InSkillKey)
{
	if(SkillTable && SkillTable->InputKey == InSkillKey)
	{
		Owner->StopJumping();
	}
}

bool UCfActionJump::IsEnd() const
{
	return (Owner && !Owner->GetMovementComponent()->IsFalling());
}

bool UCfActionJump::CanMoveDuring() const
{
	return Super::CanMoveDuring();
}

bool UCfActionJump::CanInputDuring() const
{
	return false;
}

bool UCfActionJump::IsSuperArmorActive() const
{
	return true;
}
