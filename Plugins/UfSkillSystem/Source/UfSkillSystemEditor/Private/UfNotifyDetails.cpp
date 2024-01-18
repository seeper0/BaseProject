// Fill out your copyright notice in the Description page of Project Settings.


#include "UfNotifyDetails.h"

#include "UfLogger.h"
#include "Animation/EditorNotifyObject.h"

FUfNotifyDetails::~FUfNotifyDetails()
{
	// Deselect
}

void FUfNotifyDetails::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	// Select
	UObject* NotifyPtr = nullptr;
	TArray< UObject* > OuterObjects;

	PropertyHandle->GetOuterObjects( OuterObjects );
	if( OuterObjects.Num() > 0 )
	{
		NotifyPtr = OuterObjects[ 0 ];
	}

	if( NotifyPtr != nullptr )
	{
		UEditorNotifyObject* EditorNotifyObject = Cast< UEditorNotifyObject >( NotifyPtr );
		if( EditorNotifyObject != nullptr )
		{
			//UUfAnimNotifyState_Hit* HitAnimNotifyState = Cast< UUfAnimNotifyState_Hit >( EditorNotifyObject->Event.NotifyStateClass );
		}
	}
}

void FUfNotifyDetails::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
}
