// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CfActionSkill.h"
#include "CfActionJump.generated.h"

/**
 * 점프, 몽타주는 없으나 동시 실행이 안되기 때문에 분리함
 */
UCLASS()
class CFSKILLSYSTEM_API UCfActionJump : public UCfActionSkill
{
	GENERATED_BODY()
	
public:
	virtual void OnBegin() override;
	virtual void OnEnd() override;
	virtual void OnButtonReleased(const ECfSkillKey InSkillKey) override;
	virtual bool IsEnd() const override;
	virtual bool CanMoveDuring() const override;
	virtual bool IsSuperArmorActive() const override;
};
