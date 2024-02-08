// Fill out your copyright notice in the Description page of Project Settings.


#include "Anims/CfWeaponAnimInstance.h"
#include "Anims/CfAnimInstance.h"

UCfAnimInstance* UCfWeaponAnimInstance::GetMainAnimInstance()
{
	if(USkeletalMeshComponent* MeshComponent = GetOwningComponent())
	{
		if(UCfAnimInstance* MainAnimInstance =  Cast<UCfAnimInstance>(MeshComponent->GetAnimInstance()))
		{
			return MainAnimInstance;
		}
	}
	return nullptr;
}
