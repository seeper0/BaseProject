// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CfAnimNotifyState_Hit.h"
#include "UfSkillSystemEditor.h"
#include "UObject/Interface.h"

class FCfNotifyDetails : public IPropertyTypeCustomization
{
public:
	virtual ~FCfNotifyDetails() override;
	virtual void CustomizeHeader( TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils ) override;
	virtual void CustomizeChildren( TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils ) override;

	bool Tick(float DeltaSeconds);
	void SetPreviewWorld(UWorld* InWorld);

private:
	FTSTicker::FDelegateHandle TickHandle;
	UPROPERTY()
	UWorld* World = nullptr;
	UPROPERTY()
	class UCfAnimNotifyState_Hit* HitAnimNotifyState = nullptr;
};
