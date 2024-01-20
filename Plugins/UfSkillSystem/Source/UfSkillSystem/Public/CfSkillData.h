// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CfSkill.h"
#include "CfSkillData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct UFSKILLSYSTEM_API FCfSkillData : public FTableRowBase
{
	GENERATED_BODY()

public:
	bool NotChain() const;
	bool CanChain(const FName CurrentRowName) const;
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
	ECfSkillKey InputKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECfInputType InputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECfMobileType MobileType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECfSkillOrientation Orientation;
	
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
