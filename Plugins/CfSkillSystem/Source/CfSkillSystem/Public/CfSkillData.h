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
struct CFSKILLSYSTEM_API FCfSkillData : public FTableRowBase
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

	UPROPERTY(Transient)
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECfWeaponType WeaponType = ECfWeaponType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECfSkillKey InputKey = ECfSkillKey::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECfInputType InputType = ECfInputType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECfMobileType MobileType = ECfMobileType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECfSkillOrientation Orientation = ECfSkillOrientation::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range = 150.0f;

#pragma region Hit
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Type"))
	ECfHitType HitType = ECfHitType::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Dir"))
	ECfHitDirection HitDirection = ECfHitDirection::None;

	// 넉백거리
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="KB)Dist"))
	float KnockBackDistance = 0.0f;

	// 넉백시간
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="KB)DistTime"))
	float KnockBackDistanceTime = 0.0f;

	// 넉백커브
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="KB)Curve"))
	UCurveFloat* KnockBackCurve;

	// 히트리액션 시간 (애니메이션보다 우선된다. 애니메이션을 자르던 마지막 프레임을 길게 늘리던 이 시간을 맞춘다.)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Reactime"))
	float HitReactionTime = 0.0f;

	// 경직시간
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HitStunDuration = 0.0f;

	// 경직시간 동안 AnimPlay 속도
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HitStunPlayRate = 1.0f;

	// 역경직시간
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HitStopDuration = 0.0f;

	// 역경직시간 동안 AnimPlay 속도
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HitStopPlayRate = 1.0f;
#pragma endregion
	
#pragma region GetMaxRequireSkill
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Req1"))
	FName RequireSkill1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Req2"))
	FName RequireSkill2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Req3"))
	FName RequireSkill3;
#pragma endregion // GetMaxRequireSkill

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Montage = nullptr;
};
