// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CfActionBase.h"
#include "SmoothMovable.h"
#include "CfActionSkill.generated.h"

/**
 * 스킬
 */
UCLASS()
class CFSKILLSYSTEM_API UCfActionSkill : public UCfActionBase, public ISmoothMovable
{
	GENERATED_BODY()
public:
	void InitSkill(ACharacter* InOwner, class UCfActionComponent* InComponent, const struct FCfSkillData* InSkillTable, const FVector& InInputDirection);
	const struct FCfSkillData* GetSkillTable() const { return SkillTable; }

	virtual FName GetActionName() const override;
	virtual FString ToString() const override;
	virtual bool CanMoveDuring() const override;
	virtual bool CanInputAutoRapid() const override;
	virtual bool HasSkillKey(ECfSkillKey InSkillKey) const override;

protected:
	virtual void OnBegin() override;
	virtual void OnTick(float DeltaTime) override;
	virtual void OnButtonReleased(const ECfSkillKey InSkillKey) override;

	const struct FCfSkillData* SkillTable = nullptr;

private:
	UPROPERTY()
	float ElapsedTime = 0.0f;
	UPROPERTY()
	bool bReachedDesiredRotation = true;
	UPROPERTY()
	bool bReleased = false;

	UPROPERTY()
	class UCfOverlayLockOnComponent* Target; 
	UPROPERTY()
	class UCfCameraBoomComponent* CameraBoomComponent;
	UPROPERTY()
	class UCfSkillInputComponent* InputComponent;

	UPROPERTY()
	float PrevForwardValue = 0.0f;
	UPROPERTY()
	float PrevUpValue = 0.0f;
	UPROPERTY()
	float PrevYawValue = 0.0f;

	UPROPERTY()
	FVector InputDirection;
};
