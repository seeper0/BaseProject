// Fill out your copyright notice in the Description page of Project Settings.


#include "Anims/CfAnimNotify_SwitchWeapon.h"
#include "CfLogger.h"
#include "CfUtil.h"
#include "Actions/CfActionComponent.h"

void UCfAnimNotify_SwitchWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(WeaponType != ECfWeaponType::None)
	{
		if(UCfActionComponent* Skill = UCfActionComponent::GetActionComponent(MeshComp))
		{
			CF_LOG(TEXT("SwitchWeapon: %s"), *FCfUtil::GetEnumString(WeaponType));
			Skill->SwitchWeapon(WeaponType);
		}
	}
}
