// Fill out your copyright notice in the Description page of Project Settings.


#include "CfMaterialComponent.h"

// Sets default values for this component's properties
UCfMaterialComponent::UCfMaterialComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCfMaterialComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	// 1. Init
	// TArray<UMaterialInstanceDynamic*> MI_Origins;
	// TArray<UMaterialInstanceDynamic*> MI_Changes1;
	// Mesh = InMesh;
	// //NX_LOG(TEXT("UNxMaterialUpdaterComponent : Initialize %p"), Mesh);
	// const int32 Num = Mesh->GetNumMaterials();
	// for(int32 Index = 0; Index < Num; ++Index)
	// {
	// 	// Material을 MID로 바꾸고 MI_Origins 에 넣는다.  
	// 	UMaterialInstance* Material = Cast<UMaterialInstance>(Mesh->GetMaterial(Index));
	// 	UMaterialInterface* OriginParent = Material ? Material->Parent : nullptr;
	// 	UMaterialInstanceDynamic* OldMaterialMID = UMaterialInstanceDynamic::Create(OriginParent, nullptr);
	// 	
	// 	if(Material)
	// 	{
	// 		OldMaterialMID->CopyParameterOverrides(Material);
	// 	}
	// 	Mesh->SetMaterial(Index, OldMaterialMID);
	// 	MI_Origins.Add(OldMaterialMID);
	//
	// 	// Material Parent의 Index를 찾는다,
	// 	const TArray<TObjectPtr<UMaterialInterface>>& ParentMaterialInterfaces0 = UNxTheGlobal::Get()->ParentMaterialInterfaces0;
	// 	//const int32 ParentIndex = ParentMaterialInterfaces0.Find(OriginParent);
	// 	int32 ParentIndex = INDEX_NONE;
	// 	ParentMaterialInterfaces0.FindByPredicate([OriginParent, &ParentIndex, &ParentMaterialInterfaces0](const TObjectPtr<UMaterialInterface>& Item)
	// 	{
	// 		//NX_LOG(TEXT("[%s] / [%s] / [%s]"), *Material->GetFullName(), *OriginParent->GetFullName(), *Item->GetFullName());
	// 		if(Item && OriginParent && Item->GetName() == OriginParent->GetName())
	// 		{
	// 			ParentIndex = ParentMaterialInterfaces0.Find(Item);
	// 			return true;
	// 		}
	// 		return false;
	// 	});
	//
	// 	// MI_Origins과 짝을 맞춰서 MI_Changes1를 넣는다. (nullptr이라도 넣어서) 
	// 	if(ParentIndex != INDEX_NONE)
	// 	{
	// 		UMaterialInterface* NewParent = UNxTheGlobal::Get()->ParentMaterialInterfaces1[ParentIndex];
	// 		UMaterialInstanceDynamic* NewMaterial = UMaterialInstanceDynamic::Create(NewParent, nullptr);
	// 		if(NewMaterial)
	// 		{
	// 			if(Material)
	// 			{
	// 				NewMaterial->CopyParameterOverrides(Material);
	// 			}
	// 			MI_Changes1.Add(NewMaterial);
	// 			continue;
	// 		}
	// 	}
	// 	MI_Changes1.Add(nullptr);
	// }

	// 2. PlayCurve
	// if(Mesh == nullptr)
	// 	return;
	//
	// //NX_LOG(TEXT("UNxMaterialUpdaterComponent : PlayCurveEffect %p"), Mesh);
	// ParameterName = InParameterName;
	// Curve = InCurve;
	// MaterialStyle = InMaterialStyle;
	//
	// Curve->FloatCurve.GetTimeRange( StartTime, EndTime );
	// if(InEndTime > 0.0f)
	// 	EndTime = InEndTime;
	// Timer = StartTime;
	//
	// switch(MaterialStyle)
	// {
	// case EMaterialStyle::Origin:
	// 	ChangeMaterials(MI_Origins);
	// 	break;
	// case EMaterialStyle::Masked:
	// 	ChangeMaterials(MI_Changes1);
	// 	break;
	// }
	//
	// const float Value = Curve->GetFloatValue( StartTime );
	// Mesh->SetScalarParameterValueOnMaterials(ParameterName, Value);

	// 3. ChangeMaterials
	// const int32 Num = Mesh->GetNumMaterials();
	// for(int32 Index = 0; Index < Num; ++Index)
	// {
	// 	UMaterialInstanceDynamic* OldMaterial = Cast<UMaterialInstanceDynamic>(Mesh->GetMaterial(Index));
	// 	UMaterialInstanceDynamic* NewMaterial = Materials[Index];
	// 	if(NewMaterial && OldMaterial)
	// 	{
	// 		if(NewMaterial != OldMaterial)
	// 		{
	// 			NewMaterial->CopyParameterOverrides(OldMaterial);
	// 			Mesh->SetMaterial(Index, NewMaterial);
	// 		}
	// 	}
	// }
}


// Called every frame
void UCfMaterialComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

