// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UfActionBase.h"
#include "UfActionSkill.generated.h"

/**
 * 
 */
UCLASS()
class UFSKILLSYSTEM_API UUfActionSkill : public UUfActionBase
{
	GENERATED_BODY()
public:
	void InitSkill(ACharacter* InOwner, class UUfSkillComponent* InComponent, const struct FUfSkillData* InSkillTable);
	const struct FUfSkillData* GetSkillTable() const { return SkillTable; }
	
	virtual FName GetActionName() const override;
	virtual FString ToString() const override;
	virtual bool CanMoveDuring() const override;
	virtual bool CanInputDuring() const override;

protected:
	const struct FUfSkillData* SkillTable = nullptr;
};
