// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CfSkill.h"
#include "CfCharacterData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct CFSKILLSYSTEM_API FCfCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
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

	UPROPERTY(Transient)
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECfWeaponType WeaponType1 = ECfWeaponType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DefaultWeaponKey1 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECfWeaponType WeaponType2 = ECfWeaponType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DefaultWeaponKey2 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* KnockBackMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* DownMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AirborneMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* WakeupMontage = nullptr;
};
