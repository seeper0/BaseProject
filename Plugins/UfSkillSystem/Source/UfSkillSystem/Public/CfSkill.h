// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CfSkill.generated.h"

UENUM(BlueprintType)
enum class ECfSkillState : uint8
{
	None,		/// 스킬을 실행하지 않은 상태
	PreDelay,	/// 스킬이 시작된 상태
	PreInput,	/// 선입력을 받을 수 있는 상태
	CanCancel,	/// 스킬을 캔슬 할 수 있는 상태
	End,
	Over
};

UENUM(BlueprintType)
enum class ECfInputType : uint8
{
	None,
	AutoRapid,
	Charge,
	Single,
};

UENUM(BlueprintType)
enum class ECfSkillKey : uint8
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
enum class ECfMobileType : uint8
{
	None,
	Montage,
	Input,
	Curve,
};

UENUM(BlueprintType)
enum class ECfSkillOrientation : uint8
{
	None,				// 추적 무시하고 즉시 사용한다.
	AimOriented,		// Aim이 가리키는곳 또는 LockOn 된곳을 추적한다.
	ForwardOriented,	// 캐릭터의 전방에서 추척이 된다.
};

UENUM(BlueprintType)
enum class ECfHitType : uint8
{
	None,
	Knockback,
	Down,
	Airborne,
};

UENUM(BlueprintType)
enum class ECfHitDirection : uint8
{
	None,
	Backward,
	Radial,
};

USTRUCT(BlueprintType)
struct UFSKILLSYSTEM_API FCfHitData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECfHitType HitType = ECfHitType::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECfHitDirection HitDirection = ECfHitDirection::None;
};

UENUM(BlueprintType)
enum class ECfHitShapeType : uint8
{
	None,
	Box,
	Fan,
	Donut,
};

UENUM(BlueprintType)
enum class ECfHitFlowType : uint8
{
	None,
	Instant,
	// BackToFront,
	// LeftToRight,
	// RightToLeft,
	// InwardToOutward,
};


USTRUCT(BlueprintType)
struct UFSKILLSYSTEM_API FCfHitShape
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECfHitShapeType ShapeType = ECfHitShapeType::Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=( EditCondition="ShapeType==EUfHitShapeType::Box || ShapeType==EUfHitShapeType::Fan", EditConditionHides ))
	ECfHitFlowType FlowType = ECfHitFlowType::Instant;

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
