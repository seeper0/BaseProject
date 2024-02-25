// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CfSkill.h"
#include "CfAnimLocomotionData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct CFSKILLSYSTEM_API FCfAnimLocomotionData : public FTableRowBase
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	FName CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	ECfWeaponType WeaponType = ECfWeaponType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* Idle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* Walk;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* WalkFwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* WalkFwdLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* WalkFwdRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* WalkLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* WalkRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* WalkBwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* WalkBwdLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* WalkBwdRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* RunFwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* RunFwdLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* RunFwdRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* RunLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* RunRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* RunBwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* RunBwdLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* RunBwdRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_Jump", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* JumpStart;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_Jump", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* JumpLoop;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_Jump", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* JumpEnd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_AimOffset", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* AimOffsetCC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_AimOffset", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* AimOffsetCU;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_AimOffset", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* AimOffsetCD;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_AimOffset", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* AimOffsetLC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_AimOffset", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* AimOffsetLU;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_AimOffset", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* AimOffsetLD;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_AimOffset", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* AimOffsetRC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_AimOffset", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* AimOffsetRU;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence_AimOffset", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* AimOffsetRD;
};
