// Fill out your copyright notice in the Description page of Project Settings.


#include "Anims/CfAnimNotify_SkillEnd.h"
#include "Actions/CfActionComponent.h"

UCfAnimNotify_SkillEnd::UCfAnimNotify_SkillEnd(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(255, 0, 255, 255);
#endif // WITH_EDITORONLY_DATA
}

void UCfAnimNotify_SkillEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(UCfActionComponent* Skill = UCfActionComponent::GetActionComponent(MeshComp))
	{
		Skill->SetSkillState(ECfSkillState::End);
	}
}
