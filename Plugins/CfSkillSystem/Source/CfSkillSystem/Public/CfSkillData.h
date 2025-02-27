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
	bool CanChain(const FName CurrentRowName, const bool bJustTiming) const;
	bool IsDirectionValid(const ECfCardinalDirection Direction) const;

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

#pragma region Base
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Base)
	FName CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Base)
	ECfWeaponType WeaponType = ECfWeaponType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Base)
	ECfSkillKey InputKey = ECfSkillKey::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Base)
	ECfInputType InputType = ECfInputType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Base)
	ECfMobileType MobileType = ECfMobileType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Base)
	ECfSkillOrientation Orientation = ECfSkillOrientation::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Base)
	float LockingRange = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Base)
	float DamageRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Base)
	UAnimMontage* Montage = nullptr;
#pragma endregion

#pragma region Hit
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Hit, meta=(DisplayName="Type"))
	ECfHitType HitType = ECfHitType::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Hit, meta=(DisplayName="Dir"))
	ECfHitDirection HitDirection = ECfHitDirection::None;

	// 넉백거리
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Hit, meta=(DisplayName="KB)Dist"))
	float KnockBackDistance = 0.0f;

	// 넉백시간
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Hit, meta=(DisplayName="KB)DistTime"))
	float KnockBackDistanceTime = 0.0f;

	// 넉백커브
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Hit, meta=(DisplayName="KB)Curve"))
	UCurveFloat* KnockBackCurve;

	// 히트리액션 시간 (애니메이션보다 우선된다. 애니메이션을 자르던 마지막 프레임을 길게 늘리던 이 시간을 맞춘다.)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Hit, meta=(DisplayName="Reactime"))
	float HitReactionTime = 0.0f;

	// 경직시간
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Hit)
	float HitStunDuration = 0.0f;

	// 경직시간 동안 AnimPlay 속도
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Hit)
	float HitStunPlayRate = 1.0f;

	// 역경직시간
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Hit)
	float HitStopDuration = 0.0f;

	// 역경직시간 동안 AnimPlay 속도
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Hit)
	float HitStopPlayRate = 1.0f;
#pragma endregion

#pragma region RequireSkill
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Require, meta=(DisplayName="Req1"))
	FName RequireSkill1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Require, meta=(DisplayName="Just1"))
	bool bJustSkill1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Require, meta=(DisplayName="Req2"))
	FName RequireSkill2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Require, meta=(DisplayName="Just2"))
	bool bJustSkill2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Require, meta=(DisplayName="Req3"))
	FName RequireSkill3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Require, meta=(DisplayName="Just3"))
	bool bJustSkill3;

	FName GetRequireSkill(int32 Index) const;
	bool IsJustSkill(int32 Index) const;
	int32 GetMaxRequireSkill() const { return 3; }
#pragma endregion // GetMaxRequireSkill

#pragma region Skill(Move)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float SkillMoveRange = 0.0f;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	// float SkillStopRange = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float SkillMoveTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	ECfCardinalDirection SkillDefaultDirection = ECfCardinalDirection::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	ECfCardinalDirection SkillMoveDirection = ECfCardinalDirection::None; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Move")
	UCurveFloat* SkillMoveCurve;
#pragma endregion	

#pragma region Skill(Charge)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge")
	float ChargeInputTime1 = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge")
	float ChargeDamageRate1 = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge")
	float ChargeInputTime2 = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge")
	float ChargeDamageRate2 = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge")
	float ChargeInputTime3 = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge")
	float ChargeDamageRate3 = 1.0f;

	float GetChargeInputTime(int32 Index) const;
	float GetChargeDamageRate(int32 Index) const;
	int32 GetMaxCharge() const { return 3; }
	int32 GetChargeLevel(float ChargeTime) const;
#pragma endregion
};
