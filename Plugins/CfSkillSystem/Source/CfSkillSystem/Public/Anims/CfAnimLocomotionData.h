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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_Idle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_Walk;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_WalkFwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_WalkFwdLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_WalkFwdRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_WalkLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_WalkRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_WalkBwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_WalkBwdLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_WalkBwdRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_RunFwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_RunFwdLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_RunFwdRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_RunLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_RunRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_RunBwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_RunBwdLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_RunBwdRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Jump", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_JumpStart;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Jump", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_JumpLoop;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Jump", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_JumpEnd;
};
