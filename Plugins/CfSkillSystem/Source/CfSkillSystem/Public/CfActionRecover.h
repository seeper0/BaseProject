// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CfActionBase.h"
#include "CfActionRecover.generated.h"

/**
 * 
 */
UCLASS()
class CFSKILLSYSTEM_API UCfActionRecover : public UCfActionBase
{
	GENERATED_BODY()

public:
	void InitRecover(ACharacter* InOwner, UCfActionComponent* InComponent);

	virtual bool IsSuperArmorActive() const override;
};
