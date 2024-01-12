// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EUfSkillState : uint8
{
	None,
	PreDelay,
	PreInput,
	Cancel,
	End
};

UENUM(BlueprintType)
enum class EUfInputType : uint8
{
	None,
	AutoCombo,
	Charge,
	Single,
};

UENUM(BlueprintType)
enum class EUfSkillKey : uint8
{
	None,
	NormalAttack,
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

