// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FCfRootMotionExtractorModule : public IModuleInterface
{
private:
	struct FCurveElem
	{
		FSmartName Curve;

		bool operator==(const FCurveElem& CurveElement) const
		{
			return Curve == CurveElement.Curve;
		}

		friend FORCEINLINE uint32 GetTypeHash(const FCurveElem& CurveElement)
		{
			return FCrc::MemCrc32(&CurveElement, sizeof(FCurveElem));
		}
	};

	struct FDataStruct
	{
		float KeyTime;
		float KeyValue;
	};

	struct FJob
	{
		TMap<FCurveElem, TArray<FDataStruct>> Data;
		FAssetData AssetData = nullptr;
	};

public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** This function will be bound to Command. */
	static void OnButtonClicked(FCfRootMotionExtractorModule* InModule);

private:
	void RegisterMenus();
	void ShowWindow();
	void OnWindowClosed(const TSharedRef<SWindow>& ClosedWindow);
	TSharedRef<SWidget> GenerateWindow();

	FString GetMontageAnimAssetName() const;
	void OnMontageAssetSelected(const FAssetData & AssetData);
	UClass * OnGetClassesForSourcePicker();
	bool FilterSourcePicker(const FAssetData & AssetData) const;
	FReply ButtonClicked();

	void GetRootMotionCurvesFromMontage(UAnimMontage* Montage, TArray<float>& ForwardMovements, TArray<float>& UpMovements, TArray<float>& YawRotations, TArray<float>& KeyTimes) const;
	void WriteMontage(UAnimMontage* Montage, const TArray<float>& ForwardMovements, const TArray<float>& UpMovements, const TArray<float>& YawRotations, const TArray<float>& KeyTimes);

	TSharedPtr<class SWindow> Window;
	FAssetData MontageFile;

private:
	static constexpr float FRAME_RATE = 30.0f; // 30 프레임 기준
};
