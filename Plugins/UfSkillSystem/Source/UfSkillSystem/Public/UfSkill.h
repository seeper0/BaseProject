// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputAction.h"

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
