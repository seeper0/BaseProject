// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UfSkill.generated.h"

UENUM(BlueprintType)
enum class EUfSkillState : uint8
{
	None,		/// 스킬을 실행하지 않은 상태
	PreDelay,	/// 스킬이 시작된 상태
	PreInput,	/// 선입력을 받을 수 있는 상태
	CanCancel,	/// 스킬을 캔슬 할 수 있는 상태
	End,
	Over
};

UENUM(BlueprintType)
enum class EUfInputType : uint8
{
	None,
	AutoRapid,
	Charge,
	Single,
};

UENUM(BlueprintType)
enum class EUfSkillKey : uint8
{
	None,
	Move,
	Look,
	Attack,
	Jump,
	Slot1,
	Slot2,
	Slot3,
	Slot4,
};

UENUM(BlueprintType)
enum class EUfMobileType : uint8
{
	None,
	Montage,
	Input,
	Curve,
};

UENUM(BlueprintType)
enum class EUfSkillOrientation : uint8
{
	None,				// 추적 무시하고 즉시 사용한다.
	AimOriented,		// Aim이 가리키는곳 또는 LockOn 된곳을 추적한다.
	ForwardOriented,	// 캐릭터의 전방에서 추척이 된다.
};

UENUM(BlueprintType)
enum class EUfHitType : uint8
{
	None,
	Knockback,
	Down,
	Airborne,
};

UENUM(BlueprintType)
enum class EUfHitDirection : uint8
{
	None,
	Backward,
	Radial,
};

USTRUCT(BlueprintType)
struct UFSKILLSYSTEM_API FUfHitData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EUfHitType HitType = EUfHitType::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EUfHitDirection HitDirection = EUfHitDirection::None;
};

UENUM(BlueprintType)
enum class EUfHitShapeType : uint8
{
	None,
	Box,
	Fan,
	Donut,
};

UENUM(BlueprintType)
enum class EUfHitFlowType : uint8
{
	None,
	Instant,
	// BackToFront,
	// LeftToRight,
	// RightToLeft,
	// InwardToOutward,
};


USTRUCT(BlueprintType)
struct UFSKILLSYSTEM_API FUfHitShape
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUfHitShapeType ShapeType = EUfHitShapeType::Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=( EditCondition="ShapeType==EUfHitShapeType::Box || ShapeType==EUfHitShapeType::Fan", EditConditionHides ))
	EUfHitFlowType FlowType = EUfHitFlowType::Instant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=( EditCondition="ShapeType==EUfHitShapeType::Box", EditConditionHides, DisplayName="Size" ))
	FVector2D BoxSize = FVector2D::One() * 40.0f;
	FVector GetBoxSize() const { return FVector(BoxSize.X, BoxSize.Y, HalfHeight); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="1", EditCondition="ShapeType==EUfHitShapeType::Fan || ShapeType==EUfHitShapeType::Donut", EditConditionHides, DisplayName="Radius"))
	float Radius = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", EditCondition="ShapeType==EUfHitShapeType::Donut", EditConditionHides, DisplayName="InnerRadius"))
	float InnerRadius = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", EditCondition="ShapeType==EUfHitShapeType::Fan", EditConditionHides, DisplayName="Angle"))
	float Angle = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="1", DisplayName="HalfHeight"))
	float HalfHeight = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Location"))
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Rotation"))
	FRotator Rotation = FRotator::ZeroRotator;
};
