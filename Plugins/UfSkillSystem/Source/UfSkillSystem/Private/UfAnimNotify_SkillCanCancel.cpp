// Fill out your copyright notice in the Description page of Project Settings.


#include "UfAnimNotify_SkillCanCancel.h"
#include "UfSkillComponent.h"

UUfAnimNotify_SkillCanCancel::UUfAnimNotify_SkillCanCancel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(255, 0, 255, 255);
#endif // WITH_EDITORONLY_DATA
}

void UUfAnimNotify_SkillCanCancel::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(UUfSkillComponent* Skill = UUfSkillComponent::GetSkillComponent(MeshComp))
	{
		Skill->SetSkillState(EUfSkillState::CanCancel);
	}
}
