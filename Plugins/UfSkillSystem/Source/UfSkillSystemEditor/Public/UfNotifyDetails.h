// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UfAnimNotifyState_Hit.h"
#include "UfSkillSystemEditor.h"
#include "UObject/Interface.h"

class FUfNotifyDetails : public IPropertyTypeCustomization
{
public:
	virtual ~FUfNotifyDetails() override;
	virtual void CustomizeHeader( TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils ) override;
	virtual void CustomizeChildren( TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils ) override;

	bool Tick(float DeltaSeconds);
	void SetPreviewWorld(UWorld* InWorld);

private:
	FTSTicker::FDelegateHandle TickHandle;
	UPROPERTY()
	UWorld* World = nullptr;
	UPROPERTY()
	FUfHitShape HitShape;
};
