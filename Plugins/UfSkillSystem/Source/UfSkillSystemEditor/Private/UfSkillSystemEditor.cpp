// Copyright Epic Games, Inc. All Rights Reserved.

#include "UfSkillSystemEditor.h"

#include "UfLogger.h"
#include "Editor/Persona/Public/PersonaModule.h"
#include "Editor/Persona/Public/IPersonaPreviewScene.h"
#include "Editor/Persona/Private/AnimationEditorPreviewScene.h"
#include "UfNotifyDetails.h"

#define LOCTEXT_NAMESPACE "FUfSkillSystemEditorModule"

void FUfSkillSystemEditorModule::StartupModule()
{
	if ( FModuleManager::Get().IsModuleLoaded( "Persona" ) )
	{
		FPersonaModule& PersonaModule = FModuleManager::GetModuleChecked< FPersonaModule >( "Persona" );
		PreviewSceneCreatedHandle = PersonaModule.OnPreviewSceneCreated().AddRaw( this, &FUfSkillSystemEditorModule::OnPreviewSceneCreated );
	}

	const FName PropertyEditor(TEXT("PropertyEditor"));
	if ( FModuleManager::Get().IsModuleLoaded( PropertyEditor ) )
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked< FPropertyEditorModule >( PropertyEditor );

		const FName PropertyTypeName(TEXT("AnimNotifyEvent"));
		PropertyModule.RegisterCustomPropertyTypeLayout( PropertyTypeName, FOnGetPropertyTypeCustomizationInstance::CreateRaw( this, &FUfSkillSystemEditorModule::OnAnimNotifyEvent ) );
	}
}

void FUfSkillSystemEditorModule::ShutdownModule()
{
	const FName PropertyEditor(TEXT("PropertyEditor"));
	if ( FModuleManager::Get().IsModuleLoaded( PropertyEditor ) )
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked< FPropertyEditorModule >( PropertyEditor );

		const FName PropertyTypeName(TEXT("AnimNotifyEvent"));
		PropertyModule.UnregisterCustomPropertyTypeLayout(PropertyTypeName);
	}

	if ( FModuleManager::Get().IsModuleLoaded( "Persona" ) )
	{
		FPersonaModule& PersonaModule = FModuleManager::GetModuleChecked< FPersonaModule >( "Persona" );
		if ( PreviewSceneCreatedHandle.IsValid() )
		{
			PersonaModule.OnPreviewSceneCreated().Remove( PreviewSceneCreatedHandle );
			PreviewSceneCreatedHandle.Reset();
			PreviewSceneWorld = nullptr;
		}
	}
}

#pragma region OnPreviewSceneCreated
void FUfSkillSystemEditorModule::OnPreviewSceneCreated(const TSharedRef<IPersonaPreviewScene>& Shared)
{
	const TSharedRef< FAnimationEditorPreviewScene >& AnimationEditorPreviewScene = StaticCastSharedRef<FAnimationEditorPreviewScene>( Shared );
	PreviewSceneWorld = AnimationEditorPreviewScene->GetWorld();
}
#pragma endregion

#pragma region OnAnimNotifyEvent

TSharedRef<IPropertyTypeCustomization> FUfSkillSystemEditorModule::OnAnimNotifyEvent()
{
	TSharedRef< FUfNotifyDetails > NotifyDetails = MakeShareable( new FUfNotifyDetails );
	TSharedRef< IPropertyTypeCustomization > PropertyTypeCustomization = NotifyDetails;
	NotifyDetails->SetPreviewWorld(PreviewSceneWorld);
	return PropertyTypeCustomization;
}
#pragma endregion

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUfSkillSystemEditorModule, UfSkillSystemEditor)