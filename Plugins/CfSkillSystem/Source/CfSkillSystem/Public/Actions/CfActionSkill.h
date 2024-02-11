// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CfActionBase.h"
#include "CfActionSkill.generated.h"

/**
 * 스킬
 */
UCLASS()
class CFSKILLSYSTEM_API UCfActionSkill : public UCfActionBase
{
	GENERATED_BODY()
public:
	void InitSkill(ACharacter* InOwner, class UCfActionComponent* InComponent, const struct FCfSkillData* InSkillTable);
	const struct FCfSkillData* GetSkillTable() const { return SkillTable; }

	virtual FName GetActionName() const override;
	virtual FString ToString() const override;
	virtual bool CanMoveDuring() const override;
	virtual bool CanInputDuring() const override;

protected:
	virtual void OnBegin() override;
	virtual void OnTick(float DeltaTime) override;

	const struct FCfSkillData* SkillTable = nullptr;

private:
	bool bReachedDesiredRotation = true;
	UPROPERTY()
	class UCfCameraBoomComponent* CameraBoomComponent;
};
