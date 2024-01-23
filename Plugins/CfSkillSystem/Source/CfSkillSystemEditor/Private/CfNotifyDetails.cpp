// Fill out your copyright notice in the Description page of Project Settings.


#include "CfNotifyDetails.h"
#include "Animation/EditorNotifyObject.h"
#include "IAnimationEditor.h"
#include "IPersonaPreviewScene.h"
#include "IPersonaToolkit.h"
#include "CfLogger.h"
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
				IAssetEditorInstance* AssetEditor = GEditor->GetEditorSubsystem< UAssetEditorSubsystem >()->FindEditorForAsset( EditorNotifyObject->AnimObject, true );
				if( AssetEditor != nullptr )
				{
					IAnimationEditor* AnimEditor = static_cast<IAnimationEditor*>( AssetEditor );
					PreviewWorld = AnimEditor->GetPersonaToolkit()->GetPreviewScene()->GetWorld();
				}

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
	if(PreviewWorld && HitAnimNotifyState)
	{
		UCfAnimNotifyState_Hit::DrawHitShape(PreviewWorld, HitAnimNotifyState->GetHitShape(), FTransform::Identity);
	}
	return true;
}
