// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class ESkillState : uint8
{
	None,
	PreDelay,
	PreInput,
	Cancel,
	End
};

UENUM(BlueprintType)
enum class EInputType : uint8
{
	None,
	AutoCombo,
	Charge,
	Single,
};

UENUM(BlueprintType)
enum class ESkillSlot : uint8
{
	None,
	NormalAttack,
	Slot1,
	Slot2,
	Slot3,
	Slot4,
};
