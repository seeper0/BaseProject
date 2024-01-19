// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UfSkill.h"
#include "UfActionBase.generated.h"

/**
 * 
 */
UCLASS()
class UFSKILLSYSTEM_API UUfActionBase : public UObject
{
	GENERATED_BODY()
public:
	static UUfActionBase* NewSkill(ACharacter* InOwner, class UUfSkillComponent* InComponent, const struct FUfSkillData* InSkillData);
protected:
	void InitAction(ACharacter* InOwner, class UUfSkillComponent* InComponent, UAnimMontage* InMontage);
public:
	virtual FName GetActionName() const;
	virtual FString ToString() const;

	virtual void OnBegin();
	virtual void OnTick();
	virtual void OnMontageEnd();
	virtual void OnEnd();
	virtual void OnButtonReleased(const EUfSkillKey InSkillKey);

	virtual bool IsEnd() const;
	virtual bool CanMoveDuring() const;
	virtual bool CanInputDuring() const;

protected:
	UPROPERTY()
	ACharacter* Owner = nullptr;

	UPROPERTY();
	UUfSkillComponent* Component = nullptr;

	UPROPERTY();
	UAnimMontage* Montage = nullptr;
};
