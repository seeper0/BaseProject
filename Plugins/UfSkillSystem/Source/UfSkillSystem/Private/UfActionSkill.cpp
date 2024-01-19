// Fill out your copyright notice in the Description page of Project Settings.


#include "UfActionSkill.h"

#include "UfSkillData.h"

void UUfActionSkill::InitSkill(ACharacter* InOwner, UUfSkillComponent* InComponent, const FUfSkillData* InSkillTable)
{
	Super::InitAction(InOwner, InComponent, InSkillTable->Montage);
	SkillTable = InSkillTable;
}

FName UUfActionSkill::GetActionName() const
{
	if(GetSkillTable())
	{
		return GetSkillTable()->RowName;	
	}
	return Super::GetActionName();
}

FString UUfActionSkill::ToString() const
{
	if(GetSkillTable())
	{
		return GetSkillTable()->RowName.ToString();
	}
	return Super::ToString();
}

bool UUfActionSkill::CanMoveDuring() const
{
	return (Owner && SkillTable && SkillTable->MobileType == EUfMobileType::Input);
}

bool UUfActionSkill::CanInputDuring() const
{
	if(SkillTable)
	{
		switch (SkillTable->InputType)
		{
		case EUfInputType::AutoRapid:
			return true;
		default:
			return false;
		}
	}
	return false;
}
