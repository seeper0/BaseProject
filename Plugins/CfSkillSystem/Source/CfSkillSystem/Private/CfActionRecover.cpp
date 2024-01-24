// Fill out your copyright notice in the Description page of Project Settings.


#include "CfActionRecover.h"
#include "CfActionComponent.h"

void UCfActionRecover::InitRecover(ACharacter* InOwner, UCfActionComponent* InComponent)
{
	Super::InitAction(InOwner, InComponent, InComponent->GetWakeupMontage());
}

bool UCfActionRecover::IsSuperArmorActive() const
{
	return false;
}
