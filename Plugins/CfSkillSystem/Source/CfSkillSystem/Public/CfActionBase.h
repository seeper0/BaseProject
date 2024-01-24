// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CfSkill.h"
#include "CfActionBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class CFSKILLSYSTEM_API UCfActionBase : public UObject
{
	GENERATED_BODY()
public:
	static UCfActionBase* NewSkill(ACharacter* InOwner, class UCfActionComponent* InComponent, const struct FCfSkillData* InSkillData);
	static UCfActionBase* NewHitReaction(ACharacter* InOwner, class UCfActionComponent* InComponent, const FCfDamageEvent& DamageEvent);
	static UCfActionBase* NewRecover(ACharacter* InOwner, class UCfActionComponent* InComponent);
protected:
	void InitAction(ACharacter* InOwner, class UCfActionComponent* InComponent, UAnimMontage* InMontage);
public:
	virtual FName GetActionName() const;
	virtual FString ToString() const;

	void Begin();
	void Tick(float DeltaTime);
	void End();
	void ReleaseButton(const ECfSkillKey InSkillKey);
	
protected:
	virtual void OnBegin();
	virtual void OnTick(float DeltaTime);
	virtual void OnEnd();
	virtual void OnButtonReleased(const ECfSkillKey InSkillKey);

public:
	virtual bool IsEnd() const;
	virtual bool CanMoveDuring() const;
	virtual bool CanInputDuring() const;
	virtual bool IsSuperArmorActive() const;

protected:
	UPROPERTY()
	ACharacter* Owner = nullptr;

	UPROPERTY();
	UCfActionComponent* Component = nullptr;

	UPROPERTY();
	UAnimMontage* Montage = nullptr;

#pragma region Stun
public:
	void SetStun(const float InRecoveryTime, const float InStunPlayRate);
protected:
	float GetStunPlayRate() const { return StunPlayRate; }
private:
	void TickAnimStun(float DeltaTime);

	float ElapsedRecoveryTime = 0.0f;
	float RecoveryTime = 0.0f;
	float StunPlayRate = 1.0f;
	bool IsStunned = false; 
#pragma endregion
};
