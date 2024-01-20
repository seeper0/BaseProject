// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// GameScene 관리와 달성 조건
// 1. 정상적 플레이가 되어야함 (GameScene 관리)
// 2. 개발 테스트용 플레이가 되어야함
// 3. 빌드에서도 테스트용 플레이가 되어야함

class FCfGameSceneSystemModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
