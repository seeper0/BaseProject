// Fill out your copyright notice in the Description page of Project Settings.


#include "CfSkillAsset.h"
#include "CfCharacterData.h"
#include "CfLogger.h"
#include "CfWeaponData.h"
#include "CfSkill.h"
#include "Anims/CfAnimLocomotionData.h"

void UCfSkillAsset::Initialize(const FString& InSkillAssetPath)
{
	SkillAssetPath = FSoftObjectPath(InSkillAssetPath);
}

UCfSkillAsset* UCfSkillAsset::GetInstance()
{
	if(Instance == nullptr)
	{
		Instance = Cast<UCfSkillAsset>(SkillAssetPath.TryLoad());
		if(Instance == nullptr)
		{
			CF_LOG_ERROR(TEXT("SkillAsset not found"));
		}
	}
	return Instance;
}

const FCfAnimLocomotionData* UCfSkillAsset::GetLocomotionData(const FName& CharacterName, ECfWeaponType WeaponType, const TCHAR* ContextString)
{
	if(GetLocomotionTable() == nullptr)
		return nullptr;

	CF_TODO("데이터 많아지만 문제가 될 부분. 일단 그냥 진행한다.");
	TArray<FName> RowNames = GetLocomotionTable()->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		const FCfAnimLocomotionData* RowData = GetLocomotionTable()->FindRow<FCfAnimLocomotionData>(RowName, ContextString);
		if(RowData->CharacterName == CharacterName && RowData->WeaponType == WeaponType)
		{
			return RowData;
		}
	}
	return nullptr;
}

const FCfCharacterData* UCfSkillAsset::GetCharacterData(const FName& CharacterName, const TCHAR* ContextString)
{
	if(GetCharacterTable() == nullptr)
		return nullptr;
	
	const TArray<FName> RowNames = GetCharacterTable()->GetRowNames();
	for(const FName& RowName : RowNames)
	{
		const FCfCharacterData* Data = GetCharacterTable()->FindRow<FCfCharacterData>(RowName, ContextString);
		if(Data->CharacterName == CharacterName)
		{
			return Data;
		}
	}
	return nullptr;
}

const FCfWeaponData* UCfSkillAsset::GetWeaponData(int32 WeaponKey, const TCHAR* ContextString)
{
	if(GetWeaponTable() == nullptr)
		return nullptr;

	const TArray<FName> RowNames = GetWeaponTable()->GetRowNames();
	for(const FName& RowName : RowNames)
	{
		const FCfWeaponData* Data = GetWeaponTable()->FindRow<FCfWeaponData>(RowName, ContextString);
		FString Tmp = Data->RowName.ToString();
		if(Tmp.IsNumeric())
		{
			if(FCString::Atoi(*Tmp) == WeaponKey)
			{
				return Data;
			}
		}
	}
	return nullptr;
}
