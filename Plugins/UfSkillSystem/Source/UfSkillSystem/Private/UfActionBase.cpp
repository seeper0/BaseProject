// Fill out your copyright notice in the Description page of Project Settings.


#include "UfActionBase.h"
#include "GameFramework/Character.h"

void UUfActionBase::InitAction(ACharacter* InOwner, UUfSkillComponent* InComponent, UAnimMontage* InMontage)
{
	Owner = InOwner;
	Component = InComponent;
	Montage = InMontage;
}

void UUfActionBase::OnBegin()
{
	if(Owner && Montage)
	{
		Owner->PlayAnimMontage(Montage);
	}
}

void UUfActionBase::OnTick()
{
}

void UUfActionBase::OnMontageEnd()
{
}

void UUfActionBase::OnEnd()
{
}

bool UUfActionBase::IsEnd() const
{
	// if(Montage)
	// {
	// 	Montage->Is
	// }
	return false;
}
