// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/DamageEvents.h"
#include "CfSkill.generated.h"

UENUM(BlueprintType)
enum class ECfSkillState : uint8
{
	None,		/// 스킬을 실행하지 않은 상태
	NoInput,	/// 선딜이거나 히트리액션이라 아무것도 입력할수 없는 상태
	PreInput,	/// 선입력을 받을 수 있는 상태
	CanCancel,	/// 스킬을 캔슬 할 수 있는 상태
	End,		/// 스킬이 종료된 상태. 이동 키로도 캔슬 가능하다.
	Over,		/// 스킬이 완전 종료돠어도 이전 스킬과 연계하기 위해서 있는 스테이트
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
	KnockBack,
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
struct CFSKILLSYSTEM_API FCfHitData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DamageMultiplier = 1.0f;
};

UCLASS(BlueprintType, Abstract)
class CFSKILLSYSTEM_API UCfDamageType : public UDamageType
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECfHitType HitType = ECfHitType::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECfHitDirection HitDirection = ECfHitDirection::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HitBackDistance = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HitReactionTime = 0.0f;
};

USTRUCT(BlueprintType)
struct CFSKILLSYSTEM_API FCfDamageEvent : public FDamageEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FCfHitData HitData;

	const struct FCfSkillData* SkillData = nullptr; 

	UPROPERTY()
	AController* EventInstigator = nullptr;
	
	UPROPERTY()
	ACharacter* DamageCauser = nullptr;
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

UENUM(BlueprintType)
enum class ECfZOrder : uint8
{
	HUD,
	TargetInfo
};

USTRUCT(BlueprintType)
struct CFSKILLSYSTEM_API FCfHitShape
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECfHitShapeType ShapeType = ECfHitShapeType::Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=( EditCondition="ShapeType==ECfHitShapeType::Box || ShapeType==ECfHitShapeType::Fan", EditConditionHides ))
	ECfHitFlowType FlowType = ECfHitFlowType::Instant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=( EditCondition="ShapeType==ECfHitShapeType::Box", EditConditionHides, DisplayName="Size" ))
	FVector2D BoxSize = FVector2D::One() * 40.0f;
	FVector GetBoxSize() const { return FVector(BoxSize.X, BoxSize.Y, HalfHeight); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="1", EditCondition="ShapeType==ECfHitShapeType::Fan || ShapeType==ECfHitShapeType::Donut", EditConditionHides, DisplayName="Radius"))
	float Radius = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", EditCondition="ShapeType==ECfHitShapeType::Donut", EditConditionHides, DisplayName="InnerRadius"))
	float InnerRadius = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", EditCondition="ShapeType==ECfHitShapeType::Fan", EditConditionHides, DisplayName="Angle"))
	float Angle = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="1", DisplayName="HalfHeight"))
	float HalfHeight = 50.f;

	FTransform GetTransform() const { return FTransform(Rotation, Location); }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Location"))
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Rotation"))
	FRotator Rotation = FRotator::ZeroRotator;
};
