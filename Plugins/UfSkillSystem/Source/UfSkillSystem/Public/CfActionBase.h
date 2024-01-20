// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CfSkill.h"
#include "CfActionBase.generated.h"

/**
 * 
 */
UCLASS()
class UFSKILLSYSTEM_API UCfActionBase : public UObject
{
	GENERATED_BODY()
public:
	static UCfActionBase* NewSkill(ACharacter* InOwner, class UCfSkillComponent* InComponent, const struct FCfSkillData* InSkillData);
protected:
	void InitAction(ACharacter* InOwner, class UCfSkillComponent* InComponent, UAnimMontage* InMontage);
public:
	virtual FName GetActionName() const;
	virtual FString ToString() const;

	virtual void OnBegin();
	virtual void OnTick();
	virtual void OnMontageEnd();
	virtual void OnEnd();
	virtual void OnButtonReleased(const ECfSkillKey InSkillKey);

	virtual bool IsEnd() const;
	virtual bool CanMoveDuring() const;
	virtual bool CanInputDuring() const;

protected:
	UPROPERTY()
	ACharacter* Owner = nullptr;

	UPROPERTY();
	UCfSkillComponent* Component = nullptr;

	UPROPERTY();
	UAnimMontage* Montage = nullptr;
};
