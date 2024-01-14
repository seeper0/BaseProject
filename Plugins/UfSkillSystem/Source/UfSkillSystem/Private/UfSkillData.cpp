// Fill out your copyright notice in the Description page of Project Settings.


#include "UfSkillData.h"

FName FUfSkillData::GetRequireSkill(int32 Index) const
{
	switch (Index)
	{
	case 0:
		return RequireSkill1;
	case 1:
		return RequireSkill2;
	case 2:
		return RequireSkill3;
	default:
		return NAME_None;
	}
}
