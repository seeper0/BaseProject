// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UfActionBase.generated.h"

/**
 * 
 */
UCLASS()
class UFSKILLSYSTEM_API UUfActionBase : public UObject
{
	GENERATED_BODY()
public:
	virtual void InitAction(ACharacter* InOwner, class UUfSkillComponent* InComponent, UAnimMontage* InMontage, const struct FUfSkillData* InSkillTable /* 임시 */);
	virtual FString ToString() const;

	virtual void OnBegin();
	virtual void OnTick();
	virtual void OnMontageEnd();
	virtual void OnEnd();

	bool IsEnd() const;
	const struct FUfSkillData* GetSkillTable() const { return SkillTable; }

private:
	UPROPERTY()
	ACharacter* Owner = nullptr;

	UPROPERTY();
	UUfSkillComponent* Component = nullptr;

	UPROPERTY();
	UAnimMontage* Montage = nullptr;
	
	const struct FUfSkillData* SkillTable = nullptr;
};
