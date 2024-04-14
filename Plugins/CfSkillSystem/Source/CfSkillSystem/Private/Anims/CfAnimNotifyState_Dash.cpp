// Fill out your copyright notice in the Description page of Project Settings.


#include "Anims/CfAnimNotifyState_Dash.h"
#include "GameFramework/Character.h"
#include "CfCheatManager.h"
#include "Actions/CfActionComponent.h"
#include "Actions/CfActionSkill.h"
#include "CfLogger.h"
#include "CfSkillData.h"
#include "CfStatComponent.h"

UCfAnimNotifyState_Dash::UCfAnimNotifyState_Dash(const FObjectInitializer& ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(230, 175, 20, 255);
#endif // WITH_EDITORONLY_DATA
}

void UCfAnimNotifyState_Dash::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ACharacter* Owner = UCfActionComponent::GetComponentOwner<ACharacter>(MeshComp);
	ActionComponent = UCfActionComponent::GetActionComponent(MeshComp);
	MovementComponent = UCfActionComponent::GetComponentByClass<UCharacterMovementComponent>(MeshComp);
	const UCfActionSkill* ActionSkill = Cast<UCfActionSkill>(ActionComponent->GetCurrentAction());

	if(ActionSkill && Owner)
	{
		SkillData = ActionSkill->GetSkillTable();
		InitMoveSmooth(Owner, SkillData->SkillMoveRange, SkillData->SkillMoveTime);
	}
}

void UCfAnimNotifyState_Dash::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	ACharacter* Owner = UCfActionComponent::GetComponentOwner<ACharacter>(MeshComp);

	if(SkillData && Owner)
	{
		TickMoveSmooth(FrameDeltaTime, Owner, SkillData->SkillMoveCurve);
	}
}

void UCfAnimNotifyState_Dash::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
