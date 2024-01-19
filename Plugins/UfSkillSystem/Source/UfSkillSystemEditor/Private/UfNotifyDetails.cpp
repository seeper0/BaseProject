// Fill out your copyright notice in the Description page of Project Settings.


#include "UfNotifyDetails.h"
#include "Animation/EditorNotifyObject.h"
#include "UfLogger.h"
#include "UfAnimNotifyState_Hit.h"

FUfNotifyDetails::~FUfNotifyDetails()
{
	// Deselect
	if (TickHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker( TickHandle );
	}
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
			if(UUfAnimNotifyState_Hit* HitAnimNotifyState = Cast< UUfAnimNotifyState_Hit >( EditorNotifyObject->Event.NotifyStateClass ))
			{
				HitShape = HitAnimNotifyState->GetHitShape();
				TickHandle = FTSTicker::GetCoreTicker().AddTicker( FTickerDelegate::CreateRaw( this, &FUfNotifyDetails::Tick ) );				
			}
		}
	}
}

void FUfNotifyDetails::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
}

bool FUfNotifyDetails::Tick(float DeltaSeconds)
{
	if(World)
	{
		UUfAnimNotifyState_Hit::DrawHitShape(World, HitShape);
	}
	return true;
}

void FUfNotifyDetails::SetPreviewWorld(UWorld* InWorld)
{
	World = InWorld;
}
