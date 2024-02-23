// Fill out your copyright notice in the Description page of Project Settings.


#include "CfSkillAsset.h"

#include "CfLogger.h"

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
