// Fill out your copyright notice in the Description page of Project Settings.


#include "CfAnimNotify_SkillPreInput.h"
#include "CfSkillComponent.h"

UCfAnimNotify_SkillPreInput::UCfAnimNotify_SkillPreInput(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(255, 0, 255, 255);
#endif // WITH_EDITORONLY_DATA
}

void UCfAnimNotify_SkillPreInput::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(UCfSkillComponent* Skill = UCfSkillComponent::GetSkillComponent(MeshComp))
	{
		Skill->SetSkillState(ECfSkillState::PreInput);
	}
}
