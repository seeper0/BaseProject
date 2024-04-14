// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CfSkill.h"
#include "SmoothMovable.h"
#include "Actions/CfActionComponent.h"
#include "CfAnimNotifyState_Dash.generated.h"

/**
 * 이 노티는 히트를 실행한다. 히트 박스를 생성한다. 지금은 Box 형태만 지원한다.
 */
UCLASS()
class CFSKILLSYSTEM_API UCfAnimNotifyState_Dash : public UAnimNotifyState, public ISmoothMovable
{
	GENERATED_BODY()
	
	UCfAnimNotifyState_Dash(const FObjectInitializer& ObjectInitializer);
	virtual FString GetNotifyName_Implementation() const override { return TEXT("Dash"); }

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY()
	UCfActionComponent* ActionComponent;
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
	const FCfSkillData* SkillData = nullptr;
};
