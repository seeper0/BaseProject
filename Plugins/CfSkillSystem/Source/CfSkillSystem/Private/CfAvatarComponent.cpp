// Fill out your copyright notice in the Description page of Project Settings.


#include "CfAvatarComponent.h"

#include "CfLogger.h"
#include "CfSkillAsset.h"
#include "CfWeaponData.h"

// Sets default values for this component's properties
UCfAvatarComponent::UCfAvatarComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCfAvatarComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerChar = Cast<ACharacter>(GetOwner());
}

void UCfAvatarComponent::Initialize(const FCfCharacterData* CharacterData)
{
	OwnerChar = Cast<ACharacter>(GetOwner());
	if(CharacterData == nullptr)
		return;

	WeaponData1 = UCfSkillAsset::GetWeaponData(CharacterData->DefaultWeaponKey1, CF_FUNCTION);
	if(WeaponData1)
	{
		WeaponActor1 = GetWorld()->SpawnActor(WeaponData1->WeaponClass, &FTransform::Identity);
		if(WeaponActor1)
			WeaponActor1->AttachToComponent(OwnerChar->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponData1->EquipSocket);
	}
	WeaponData2 = UCfSkillAsset::GetWeaponData(CharacterData->DefaultWeaponKey2, CF_FUNCTION);
	if(WeaponData2)
	{
		WeaponActor2 = GetWorld()->SpawnActor(WeaponData2->WeaponClass, &FTransform::Identity);
		if(WeaponActor2)
			WeaponActor2->AttachToComponent(OwnerChar->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponData2->UnEquipSocket);
	}
}

// Called every frame
void UCfAvatarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

