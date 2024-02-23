// Fill out your copyright notice in the Description page of Project Settings.


#include "UfEngineSubsystem.h"
#include "CfLogger.h"
#include "CfSkillAsset.h"


void UUfEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UCfSkillAsset::Initialize(TEXT("/Game/Tables/DA_Skill"));
}
