// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UfSkill.h"
#include "UfSkillTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct UFSKILLSYSTEM_API FUfSkillTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUfSkillKey Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUfInputType InputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUfMobileType MobileType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Montage = nullptr;
};
