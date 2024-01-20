// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FCfSkillSystemEditorModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
#pragma region OnPreviewSceneCreated
	void OnPreviewSceneCreated(const TSharedRef<class IPersonaPreviewScene>& Shared);
	UPROPERTY()
	FDelegateHandle PreviewSceneCreatedHandle;
	UPROPERTY()
	UWorld* PreviewSceneWorld = nullptr;
#pragma endregion
	
#pragma region OnAnimNotifyEvent
	TSharedRef< IPropertyTypeCustomization > OnAnimNotifyEvent();
	uint32 AnimNotifyId = 0;
#pragma endregion
};
