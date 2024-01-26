// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CfActionBase.h"
#include "CfActionRecovery.generated.h"

/**
 * 히트 리액션 후 복구 몽타주가 별도로 필요할때 사용한다. (ex : Getup)
 */
UCLASS()
class CFSKILLSYSTEM_API UCfActionRecovery : public UCfActionBase
{
	GENERATED_BODY()

public:
	void InitRecover(ACharacter* InOwner, UCfActionComponent* InComponent);

	virtual bool IsSuperArmorActive() const override;
};
