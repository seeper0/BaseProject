// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CfAnimNotify_SkillPreInput.generated.h"

/**
 * 
 */
UCLASS()
class UFSKILLSYSTEM_API UCfAnimNotify_SkillPreInput : public UAnimNotify
{
	GENERATED_BODY()

	UCfAnimNotify_SkillPreInput(const FObjectInitializer& ObjectInitializer);
	virtual FString GetNotifyName_Implementation() const override { return TEXT("1_PreInput"); }
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
