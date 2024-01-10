// Fill out your copyright notice in the Description page of Project Settings.


#include "UfAnimNotify_SkillCancel.h"

#include "UfSkillComponent.h"

UUfAnimNotify_SkillCancel::UUfAnimNotify_SkillCancel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(255, 0, 255, 255);
#endif // WITH_EDITORONLY_DATA
}

void UUfAnimNotify_SkillCancel::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(UUfSkillComponent* Skill = UUfSkillComponent::GetSkillComponent(MeshComp))
	{
		Skill->SetSkillState(ESkillState::Cancel);
	}
}
