// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "CfSkill.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table", meta = (AllowPrivateAccess = "true"))
	UDataTable* SkillTable = nullptr;
public:
	static const UDataTable* GetSkillTable() { return GetInstance() ? GetInstance()->SkillTable : nullptr; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table", meta = (AllowPrivateAccess = "true"))
	UDataTable* LocomotionTable = nullptr;
public:
	static const UDataTable* GetLocomotionTable() { return GetInstance() ? GetInstance()->LocomotionTable : nullptr; }
	static const struct FCfAnimLocomotionData* GetLocomotionData(const FName& CharacterName, enum class ECfWeaponType WeaponType, const TCHAR* ContextString);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table", meta = (AllowPrivateAccess = "true"))
	UDataTable* CharacterTable = nullptr;
public:
	static const UDataTable* GetCharacterTable() { return GetInstance() ? GetInstance()->CharacterTable : nullptr; }
	static const struct FCfCharacterData* GetCharacterData(const FName& CharacterName, const TCHAR* ContextString);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table", meta = (AllowPrivateAccess = "true"))
	UDataTable* WeaponTable = nullptr;
public:
	static const UDataTable* GetWeaponTable() { return GetInstance() ? GetInstance()->WeaponTable : nullptr; }
	static const struct FCfWeaponData* GetWeaponData(int32 WeaponKey, const TCHAR* ContextString);
};
