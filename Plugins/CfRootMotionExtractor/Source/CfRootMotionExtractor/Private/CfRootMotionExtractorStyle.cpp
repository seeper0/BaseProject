// Copyright Epic Games, Inc. All Rights Reserved.

#include "CfRootMotionExtractorStyle.h"
#include "CfRootMotionExtractor.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FCfRootMotionExtractorStyle::StyleInstance = nullptr;

void FCfRootMotionExtractorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FCfRootMotionExtractorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FCfRootMotionExtractorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("CfRootMotionExtractorStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FCfRootMotionExtractorStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("CfRootMotionExtractorStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("CfRootMotionExtractor")->GetBaseDir() / TEXT("Resources"));

	Style->Set("CfRootMotionExtractor.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FCfRootMotionExtractorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FCfRootMotionExtractorStyle::Get()
{
	return *StyleInstance;
}
