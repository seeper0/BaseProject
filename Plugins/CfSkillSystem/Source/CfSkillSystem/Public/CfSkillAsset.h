// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CfSkillAsset.generated.h"

/**
 * 이 클래스는 스킬과 관련된 데이터를 관리하는 클래스입니다.
 * 실제 DataAsset은 Content 폴더에서 만들어서 관리해야합니다.
 * UEngineSubsystem::Initialize에서 UCfSkillAsset::Initialize를 호출하여 초기화해야합니다. (DataAsset은 Path로 초기화)
 */
UCLASS()
class CFSKILLSYSTEM_API UCfSkillAsset : public UDataAsset
{
	GENERATED_BODY()
private:
	inline static UCfSkillAsset* Instance = nullptr;
	inline static FSoftObjectPath SkillAssetPath;

public:
	static void Initialize(const FString& InSkillAssetPath);
	static UCfSkillAsset* GetInstance();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence", meta = (AllowPrivateAccess = "true"))
	UDataTable* SkillTable = nullptr;
public:
	static const UDataTable* GetSkillTable() { return GetInstance() ? GetInstance()->SkillTable : nullptr; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence", meta = (AllowPrivateAccess = "true"))
	UDataTable* LocomotionTable = nullptr;
public:
	static const UDataTable* GetLocomotionTable() { return GetInstance() ? GetInstance()->LocomotionTable : nullptr; }
};
