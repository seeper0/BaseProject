// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UfSkill.h"
#include "UfSkillData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct UFSKILLSYSTEM_API FUfSkillData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FName GetRequireSkill(int32 Index) const;
	int32 GetMaxRequireSkill() const { return 3; }

	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems) override
	{
		FTableRowBase::OnPostDataImport(InDataTable, InRowName, OutCollectedImportProblems);
		RowName = InRowName;
	}

	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override
	{
		FTableRowBase::OnDataTableChanged(InDataTable, InRowName);
		RowName = InRowName;
	}

	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUfSkillKey Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUfInputType InputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUfMobileType MobileType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Montage = nullptr;

#pragma region GetMaxRequireSkill
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RequireSkill1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RequireSkill2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RequireSkill3;
#pragma endregion // GetMaxRequireSkill
};
