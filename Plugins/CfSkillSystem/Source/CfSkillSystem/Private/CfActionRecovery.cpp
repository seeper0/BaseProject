// Fill out your copyright notice in the Description page of Project Settings.


#include "CfActionRecovery.h"
#include "CfActionComponent.h"

void UCfActionRecovery::InitRecover(ACharacter* InOwner, UCfActionComponent* InComponent)
{
	Super::InitAction(InOwner, InComponent, InComponent->GetWakeupMontage());
}

bool UCfActionRecovery::IsSuperArmorActive() const
{
	return false;
}
