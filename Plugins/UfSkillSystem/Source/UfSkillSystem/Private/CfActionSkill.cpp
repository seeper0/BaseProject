// Fill out your copyright notice in the Description page of Project Settings.


#include "CfActionSkill.h"

#include "CfSkillData.h"

void UCfActionSkill::InitSkill(ACharacter* InOwner, UCfSkillComponent* InComponent, const FCfSkillData* InSkillTable)
{
	Super::InitAction(InOwner, InComponent, InSkillTable->Montage);
	SkillTable = InSkillTable;
}

FName UCfActionSkill::GetActionName() const
{
	if(GetSkillTable())
	{
		return GetSkillTable()->RowName;	
	}
	return Super::GetActionName();
}

FString UCfActionSkill::ToString() const
{
	if(GetSkillTable())
	{
		return GetSkillTable()->RowName.ToString();
	}
	return Super::ToString();
}

bool UCfActionSkill::CanMoveDuring() const
{
	return (Owner && SkillTable && SkillTable->MobileType == ECfMobileType::Input);
}

bool UCfActionSkill::CanInputDuring() const
{
	if(SkillTable)
	{
		switch (SkillTable->InputType)
		{
		case ECfInputType::AutoRapid:
			return true;
		default:
			return false;
		}
	}
	return false;
}
