// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CfActionBase.h"
#include "CfActionHit.generated.h"

/**
 * 
 */
UCLASS()
class CFSKILLSYSTEM_API UCfActionHit : public UCfActionBase
{
	GENERATED_BODY()

public:
	void InitHit(ACharacter* InOwner, UCfActionComponent* InComponent, const FCfDamageEvent& InDamageEvent);
	virtual FName GetActionName() const override;
	virtual FString ToString() const override;
	
	virtual void OnBegin() override;
	virtual void OnTick(float DeltaTime) override;
	virtual void OnEnd() override;
	virtual bool IsEnd() const override;

private:
	FCfDamageEvent DamageEvent;
	float RemainingTime = 0.0f;
};
