// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UfActionSkill.h"
#include "UfActionJump.generated.h"

/**
 * 
 */
UCLASS()
class UFSKILLSYSTEM_API UUfActionJump : public UUfActionSkill
{
	GENERATED_BODY()
	
public:
	virtual void OnBegin() override;
	virtual void OnEnd() override;
	virtual void OnButtonReleased(const EUfSkillKey InSkillKey) override;
	virtual bool IsEnd() const override;
	virtual bool CanMoveDuring() const override;
	virtual bool CanInputDuring() const override;
};
