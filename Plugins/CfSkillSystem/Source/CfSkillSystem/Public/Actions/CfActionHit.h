// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CfActionBase.h"
#include "CfActionHit.generated.h"

/**
 * 히트리액션
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
	virtual bool IsSuperArmorActive() const override;

private:
	UPROPERTY()
	class UCharacterMovementComponent* MovementComponent;

	UPROPERTY()
	FCfDamageEvent DamageEvent;

	UPROPERTY()
	float ElapsedTime = 0.0f;
	
	UPROPERTY()
	float ElapsedKnockBackTime = 0.0f;

#pragma region Knockback
	void InitKnockBack(const FCfDamageEvent& InDamageEvent);
	void TickKnockBack(float DeltaTime);

	FVector StartLocation;
	FVector EndLocation;
	FVector PrevLocation;
#pragma endregion	
};
