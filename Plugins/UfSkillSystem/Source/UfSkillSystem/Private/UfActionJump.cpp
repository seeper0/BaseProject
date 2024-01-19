// Fill out your copyright notice in the Description page of Project Settings.


#include "UfActionJump.h"
#include "GameFramework/Character.h"
#include "UfSkillData.h"
#include "GameFramework/PawnMovementComponent.h"

void UUfActionJump::OnBegin()
{
	if(SkillTable && SkillTable->InputKey == EUfSkillKey::Jump)
	{
		Owner->Jump();
	}
}

void UUfActionJump::OnEnd()
{
	Super::OnEnd();
}

void UUfActionJump::OnButtonReleased(const EUfSkillKey InSkillKey)
{
	if(SkillTable && SkillTable->InputKey == InSkillKey)
	{
		Owner->StopJumping();
	}
}

bool UUfActionJump::IsEnd() const
{
	return (Owner && !Owner->GetMovementComponent()->IsFalling());
}

bool UUfActionJump::CanMoveDuring() const
{
	return Super::CanMoveDuring();
}

bool UUfActionJump::CanInputDuring() const
{
	return false;
}
