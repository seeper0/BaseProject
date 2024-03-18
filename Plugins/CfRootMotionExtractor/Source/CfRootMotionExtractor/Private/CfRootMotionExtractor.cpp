// Copyright Epic Games, Inc. All Rights Reserved.

#include "CfRootMotionExtractor.h"

#include "AnimationBlueprintLibrary.h"
#include "CfLogger.h"
#include "CfRootMotionExtractorStyle.h"
#include "PropertyCustomizationHelpers.h"
#include "Widgets/SCanvas.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "AssetRegistry/AssetRegistryModule.h"

static const FName CfRootMotionExtractorTabName("CfRootMotionExtractor");

#define LOCTEXT_NAMESPACE "FCfRootMotionExtractorModule"

void FCfRootMotionExtractorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FCfRootMotionExtractorStyle::Initialize();
	FCfRootMotionExtractorStyle::ReloadTextures();

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCfRootMotionExtractorModule::RegisterMenus));
}

void FCfRootMotionExtractorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FCfRootMotionExtractorStyle::Shutdown();
}

void FCfRootMotionExtractorModule::OnButtonClicked(FCfRootMotionExtractorModule* InModule)
{
	if(InModule)
		InModule->ShowWindow();
}

void FCfRootMotionExtractorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				// 드롭 메뉴 추가
				UToolMenu* SubMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar.CfShortcut");
				SubMenu->AddSection("LevelShortcut", LOCTEXT("LevelShortcut", "Level Shortcut")); // 일단 등록함. (순서 문제)
				FToolMenuSection& SubSection = SubMenu->AddSection("Tools", LOCTEXT("Tools", "Tools"));
				SubSection.AddDynamicEntry("CfShortcut", FNewToolMenuSectionDelegate::CreateLambda([this](FToolMenuSection& InSection)
				{
					InSection.AddMenuEntry(
						NAME_None,
						FText::FromString(TEXT("Root Motion Extractor")),
						FText::FromString(TEXT("Root Motion Extractor")),
						FSlateIcon(),
						FUIAction(FExecuteAction::CreateStatic(&OnButtonClicked, this))
					);
				}));
			}
		}
	}
}

void FCfRootMotionExtractorModule::ShowWindow()
{
	if (!Window.IsValid())
	{
		SAssignNew(Window, SWindow)
			.ClientSize(FVector2D(500, 110 ))
			.CreateTitleBar(true)
			.Title(FText::FromString(TEXT("Root Motion Extractor")))
			.SupportsMaximize(false)
			.SupportsMinimize(true)
			.FocusWhenFirstShown(true)
			.IsTopmostWindow(true)
			.SizingRule(ESizingRule::FixedSize);
		Window->SetOnWindowClosed(FOnWindowClosed::CreateRaw(this, &FCfRootMotionExtractorModule::OnWindowClosed));
		Window->SetContent(GenerateWindow());
		FSlateApplication::Get().AddWindow(Window.ToSharedRef(), false);
		if (Window.IsValid())
		{
			Window->ShowWindow();
		}
	}
}

void FCfRootMotionExtractorModule::OnWindowClosed(const TSharedRef<SWindow>& ClosedWindow)
{
	Window.Reset();
}

TSharedRef<SWidget> FCfRootMotionExtractorModule::GenerateWindow()
{
	return SNew(SToolTip)
	[
		SNew(SWrapBox)
		+ SWrapBox::Slot()
		.VAlign(VAlign_Center)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.Padding(5.0f, 5.0f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.FillHeight(0.6f)
				[
					SNew(STextBlock).Text(FText::FromString(TEXT("Montage:")))
				]
				+ SVerticalBox::Slot()
				.FillHeight(1.4f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					[
						SNew(SBox)
						.HeightOverride(30.f)
						.WidthOverride(470.f)
						[
							SNew(SObjectPropertyEntryBox)
							.ObjectPath_Raw(this, &FCfRootMotionExtractorModule::GetMontageAnimAssetName)
							.OnObjectChanged_Raw(this, &FCfRootMotionExtractorModule::OnMontageAssetSelected)
							.AllowClear(false)
							.AllowedClass(FCfRootMotionExtractorModule::OnGetClassesForSourcePicker())
							.OnShouldFilterAsset_Raw(this, &FCfRootMotionExtractorModule::FilterSourcePicker)
						]
					]
				]
				+ SVerticalBox::Slot()
				.Padding(2.0f, 12.0f)
				[
					SNew(SBorder)
					.BorderBackgroundColor(FSlateColor(FLinearColor(48.0f, 48.0f, 48.0f, 1.0f)))
					[
						SNew(SButton)
						.Text_Lambda([this]() { return FText::FromString(TEXT("Extract")); })
						//.IsEnabled_Lambda([this]() { return !bIsExtracting; })
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						.OnClicked_Raw(this, &FCfRootMotionExtractorModule::ButtonClicked)
					]
				]
			]
		]
	];
}

FString FCfRootMotionExtractorModule::GetMontageAnimAssetName() const
{
	if (MontageFile.IsValid())
		return MontageFile.GetSoftObjectPath().ToString();

	return FString(TEXT(""));
}

void FCfRootMotionExtractorModule::OnMontageAssetSelected(const FAssetData& AssetData)
{
	if (AssetData.IsValid())
	{
		if(UAnimMontage *LocalSource = Cast<UAnimMontage>(AssetData.GetAsset()))
		{
			MontageFile = AssetData;
		}
	}
}

UClass* FCfRootMotionExtractorModule::OnGetClassesForSourcePicker()
{
	return UObject::StaticClass();
}

bool FCfRootMotionExtractorModule::FilterSourcePicker(const FAssetData& AssetData) const
{
	if(!AssetData.IsValid())
		return true;

	if(!AssetData.GetClass()->IsValidLowLevel())
		return true;

	if (AssetData.GetClass()->IsChildOf(UAnimMontage::StaticClass()))
		return false;

	return true;
}

FReply FCfRootMotionExtractorModule::ButtonClicked()
{
	if(UAnimMontage* Montage = Cast<UAnimMontage>(MontageFile.GetAsset()))
	{
		TArray<float> ForwardMovements, UpMovements, YawRotations, KeyTimes;
		GetRootMotionCurvesFromMontage(Montage, ForwardMovements, UpMovements, YawRotations, KeyTimes);
		WriteMontage(Montage, ForwardMovements, UpMovements, YawRotations, KeyTimes);

		const FString Mesg = FString::Printf(TEXT("Root Motion Extracted [%s]"), *MontageFile.AssetName.ToString()); 
		if(FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Mesg)) == EAppReturnType::Ok)
		{
			Window->RequestDestroyWindow();
		}
	}
	return FReply::Handled();
}

void FCfRootMotionExtractorModule::GetRootMotionCurvesFromMontage(UAnimMontage* Montage, TArray<float>& ForwardMovements, TArray<float>& UpMovements, TArray<float>& YawRotations, TArray<float>& KeyTimes) const
{
	if (!Montage)
	{
		return;
	}

	const float MontageLength = Montage->GetPlayLength();
	const int32 TotalFrames = FMath::FloorToInt(FRAME_RATE * MontageLength);

	//OutRootTransforms.Empty();
	//OutRootTransforms.Reserve(TotalFrames);

	for (int32 Frame = 0; Frame < TotalFrames; ++Frame)
	{
		const float CurrentTime = Montage->GetPlayLength() * ((float)Frame / (float)TotalFrames);
		FTransform FrameRootTransform = FTransform::Identity;

		// Montage에서 SlotAnimTracks를 순회
		for (const FSlotAnimationTrack& SlotAnimTrack : Montage->SlotAnimTracks)
		{
			for (const FAnimSegment& Segment : SlotAnimTrack.AnimTrack.AnimSegments)
			{
				if (const UAnimSequence* AnimSequence = Cast<UAnimSequence>(Segment.GetAnimReference()))
				{
					const float SegmentPlayRate = Segment.AnimPlayRate * Montage->RateScale;
					const float SegmentStartTime = Segment.StartPos;
					const float SegmentEndTime = SegmentStartTime + Segment.GetLength();

					// 현재 시간이 세그먼트 범위 내에 있는지 확인
					if (CurrentTime >= SegmentStartTime && CurrentTime <= SegmentEndTime)
					{
						const float AnimTime = (CurrentTime - SegmentStartTime) * SegmentPlayRate;
						FTransform SegmentRootTransform = AnimSequence->ExtractRootMotionFromRange(0.f, AnimTime);

						// 루트 트랜스폼을 결합
						FrameRootTransform = FrameRootTransform * SegmentRootTransform;
						// CF_LOG(TEXT("FrameRootTransform : %s"), *FrameRootTransform.GetTranslation().ToString());
					}
				}
			}
		}

		//OutRootTransforms.Add(FrameRootTransform);
		const double ForwardMovement = FrameRootTransform.GetTranslation().Y;
		const double UpMovement = FrameRootTransform.GetTranslation().Z;
		const double YawRotation = FrameRootTransform.GetRotation().GetNormalized().Euler().Z;

		ForwardMovements.Add(ForwardMovement);
		UpMovements.Add(UpMovement);
		YawRotations.Add(YawRotation);
		KeyTimes.Add(CurrentTime);
	}
}

void FCfRootMotionExtractorModule::WriteMontage(UAnimMontage* Montage, const TArray<float>& ForwardMovements, const TArray<float>& UpMovements, const TArray<float>& YawRotations, const TArray<float>& KeyTimes)
{
	const bool CanModify = Montage && Montage->CanModify();
	if(!CanModify)
		return;
	Montage->SetFlags(RF_Public | RF_Standalone);
	Montage->Modify(true);

	IAnimationDataController& Controller = Montage->GetController();
	const FAnimationCurveIdentifier ForwardMovementCurveId(TEXT("ForwardMovement"), ERawCurveTrackTypes::RCT_Float);
	const FAnimationCurveIdentifier UpMovementCurveId(TEXT("UpMovement"), ERawCurveTrackTypes::RCT_Float);
	const FAnimationCurveIdentifier YawRotationCurveId(TEXT("YawRotation"), ERawCurveTrackTypes::RCT_Float);

	Controller.RemoveCurve(ForwardMovementCurveId);
	Controller.RemoveCurve(UpMovementCurveId);
	Controller.RemoveCurve(YawRotationCurveId);
	Montage->RefreshCacheData();

	Controller.AddCurve(ForwardMovementCurveId);
	Controller.AddCurve(UpMovementCurveId);
	Controller.AddCurve(YawRotationCurveId);

	const int32 NumKeys = KeyTimes.Num();
	for (int32 KeyIndex = 0; KeyIndex < NumKeys; ++KeyIndex)
	{
		Controller.SetCurveKey(ForwardMovementCurveId, { KeyTimes[KeyIndex], ForwardMovements[KeyIndex] });
		Controller.SetCurveKey(UpMovementCurveId, { KeyTimes[KeyIndex], UpMovements[KeyIndex] });
		Controller.SetCurveKey(YawRotationCurveId, { KeyTimes[KeyIndex], YawRotations[KeyIndex] });
	}

	Montage->PostLoad();
	Montage->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(Montage);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCfRootMotionExtractorModule, CfRootMotionExtractor)