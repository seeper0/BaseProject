// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Anims/CfAnimNotifyState_Hit.h"
#include "CfSkillSystemEditor.h"
#include "UObject/Interface.h"

class FCfNotifyDetails : public IPropertyTypeCustomization
{
public:
	virtual ~FCfNotifyDetails() override;
	virtual void CustomizeHeader( TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils ) override;
	virtual void CustomizeChildren( TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils ) override;

	bool Tick(float DeltaSeconds);

private:
	FTSTicker::FDelegateHandle TickHandle;
	UPROPERTY()
	UWorld* PreviewWorld = nullptr;
	UPROPERTY()
	class UCfAnimNotifyState_Hit* HitAnimNotifyState = nullptr;
};
