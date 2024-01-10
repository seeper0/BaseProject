// Fill out your copyright notice in the Description page of Project Settings.


#include "UfAnimNotify_SkillInput.h"
#include "UfSkillComponent.h"

UUfAnimNotify_SkillInput::UUfAnimNotify_SkillInput(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(255, 0, 255, 255);
#endif // WITH_EDITORONLY_DATA
}

void UUfAnimNotify_SkillInput::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(UUfSkillComponent* Skill = UUfSkillComponent::GetSkillComponent(MeshComp))
	{
		Skill->SetSkillState(ESkillState::PreInput);
	}
}
