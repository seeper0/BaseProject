// Copyright Epic Games, Inc. All Rights Reserved.

#include "CfShortcut.h"
#include "FileHelpers.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "CfShortcutStyle.h"
#include "CfLogger.h"

static const FName CfShortcutTabName("CfShortcut");

#define LOCTEXT_NAMESPACE "FCfShortcutModule"

void FCfShortcutModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FCfShortcutStyle::Initialize();
	FCfShortcutStyle::ReloadTextures();
	
	GConfig->GetArray(TEXT("CfShortcut"), TEXT("FavoriteLevelPaths"), FavoriteLevelPaths, GEditorIni);

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCfShortcutModule::RegisterMenus));
}

void FCfShortcutModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FCfShortcutStyle::Shutdown();
}

void FCfShortcutModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	// 툴바에 추가
	UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
	{
		FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");

		// 툴바 버튼 추가
		FToolMenuEntry Entry = FToolMenuEntry::InitComboButton(
		  "CfShortcut",				// Name
		  FUIAction(),			// Action: default
		  FNewToolMenuChoice(),		// Contents: default
		  LOCTEXT("CfShortcut", "Shortcut"),
		  LOCTEXT("CfShortcut_Tooltip", "Shortcut for Tools"),
		  FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Star"),
		  false
		); 
		Entry.StyleNameOverride = "CalloutToolbar";
		Section.AddEntry(Entry);

		// 드롭 메뉴 추가
		UToolMenu* SubMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar.CfShortcut");
		FToolMenuSection& SubSection = SubMenu->AddSection("LevelShortcut", LOCTEXT("LevelShortcut", "Level Shortcut"));

		SubSection.AddDynamicEntry("CfShortcut", FNewToolMenuSectionDelegate::CreateLambda([this](FToolMenuSection& InSection) {
			for(const FString& FavoriteLevelPath : FavoriteLevelPaths )
			{
				const FString BaseFilename = FPaths::GetBaseFilename(FavoriteLevelPath);
				const FString Label = FString::Printf(TEXT("%s"), *BaseFilename);
				const FString Tooltip = FString::Printf(TEXT("Open %s"), *BaseFilename);

				if(BaseFilename.Len() > 0)
				{
					InSection.AddMenuEntry(
						NAME_None,
						FText::FromString(Label),
						FText::FromString(Tooltip),
						FSlateIcon(),
						FUIAction(FExecuteAction::CreateStatic(&MakeOpenLevelMenu, FavoriteLevelPath))
					);
				}
			}
		}));
	}
}

void FCfShortcutModule::MakeOpenLevelMenu(FString MapPath)
{
	FEditorFileUtils::LoadMap(MapPath);
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCfShortcutModule, CfShortcut)