// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CfSkill.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CfAnimNotify_SwitchWeapon.generated.h"

/**
 * 
 */
UCLASS()
class CFSKILLSYSTEM_API UCfAnimNotify_SwitchWeapon : public UAnimNotify
{
	GENERATED_BODY()

	virtual FString GetNotifyName_Implementation() const override { return TEXT("SwitchWeapon"); }
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECfWeaponType WeaponType = ECfWeaponType::None;
};
