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
