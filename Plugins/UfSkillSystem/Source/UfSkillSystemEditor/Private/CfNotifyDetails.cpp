// Fill out your copyright notice in the Description page of Project Settings.


#include "CfNotifyDetails.h"
#include "Animation/EditorNotifyObject.h"
#include "..\..\..\..\UfCommon\Source\UfCommon\Public\CfLogger.h"
#include "CfAnimNotifyState_Hit.h"

FCfNotifyDetails::~FCfNotifyDetails()
{
	// Deselect
	if (TickHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker( TickHandle );
	}
}

void FCfNotifyDetails::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
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
			HitAnimNotifyState = Cast< UCfAnimNotifyState_Hit >( EditorNotifyObject->Event.NotifyStateClass );
			if(HitAnimNotifyState)
			{
				TickHandle = FTSTicker::GetCoreTicker().AddTicker( FTickerDelegate::CreateRaw( this, &FCfNotifyDetails::Tick ) );				
			}
		}
	}
}

void FCfNotifyDetails::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
}

bool FCfNotifyDetails::Tick(float DeltaSeconds)
{
	if(World && HitAnimNotifyState)
	{
		UCfAnimNotifyState_Hit::DrawHitShape(World, HitAnimNotifyState->GetHitShape());
	}
	return true;
}

void FCfNotifyDetails::SetPreviewWorld(UWorld* InWorld)
{
	World = InWorld;
}
