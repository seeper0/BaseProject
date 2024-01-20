// Fill out your copyright notice in the Description page of Project Settings.


#include "CfAnimNotifyState_Hit.h"
#include "CfSkillComponent.h"

FColor UCfAnimNotifyState_Hit::HitColor = FColor(230, 175, 20, 255);

UCfAnimNotifyState_Hit::UCfAnimNotifyState_Hit(const FObjectInitializer& ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = HitColor;
#endif // WITH_EDITORONLY_DATA
}

void UCfAnimNotifyState_Hit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if(UCfSkillComponent* Skill = UCfSkillComponent::GetSkillComponent(MeshComp))
	{
		Skill->ClearHitActorList();
	}
}

void UCfAnimNotifyState_Hit::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if(UCfSkillComponent* Skill = UCfSkillComponent::GetSkillComponent(MeshComp))
	{
		
	}
	//MeshComp->GetWorld()->WorldType : EWorldType::Type::EditorPreview
}

void UCfAnimNotifyState_Hit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if(UCfSkillComponent* Skill = UCfSkillComponent::GetSkillComponent(MeshComp))
	{
	}
	// 치트 매니저 체크해서 충돌 표시가 true라면 DrawHitShape 호출
	// 치트매니저도 이쪽으로 와야하는가????
}

void UCfAnimNotifyState_Hit::DrawHitShape(UWorld* InWorld, const FCfHitShape& InHitShape)
{
	if(InWorld == nullptr)
		return;

	switch(InHitShape.ShapeType)
	{
	case ECfHitShapeType::Box:
		::DrawDebugPoint(InWorld, InHitShape.Location, 10, HitColor);
		::DrawDebugBox(InWorld, InHitShape.Location, InHitShape.GetBoxSize(), InHitShape.Rotation.Quaternion(), HitColor);
		break;
	case ECfHitShapeType::Fan:
		break;
	case ECfHitShapeType::Donut:
		break;
	default:
		break;
	}
}
