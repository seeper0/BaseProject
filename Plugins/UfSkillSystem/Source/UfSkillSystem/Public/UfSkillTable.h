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
	int32 Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInputType InputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Montage = nullptr;
};
