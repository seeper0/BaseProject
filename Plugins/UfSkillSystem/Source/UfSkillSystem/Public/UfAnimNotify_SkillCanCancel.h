// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "UfAnimNotify_SkillCanCancel.generated.h"

/**
 * 
 */
UCLASS()
class UFSKILLSYSTEM_API UUfAnimNotify_SkillCanCancel : public UAnimNotify
{
	GENERATED_BODY()
	
	UUfAnimNotify_SkillCanCancel(const FObjectInitializer& ObjectInitializer);
	virtual FString GetNotifyName_Implementation() const override { return TEXT("2_CanCancel"); }
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
