// Fill out your copyright notice in the Description page of Project Settings.


#include "UfAnimNotifyState_Hit.h"
#include "UfSkillComponent.h"

FColor UUfAnimNotifyState_Hit::HitColor = FColor(230, 175, 20, 255);

UUfAnimNotifyState_Hit::UUfAnimNotifyState_Hit(const FObjectInitializer& ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = HitColor;
#endif // WITH_EDITORONLY_DATA
}

void UUfAnimNotifyState_Hit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if(UUfSkillComponent* Skill = UUfSkillComponent::GetSkillComponent(MeshComp))
	{
		Skill->ClearHitActorList();
	}
}

void UUfAnimNotifyState_Hit::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if(UUfSkillComponent* Skill = UUfSkillComponent::GetSkillComponent(MeshComp))
	{
		
	}
	//MeshComp->GetWorld()->WorldType : EWorldType::Type::EditorPreview
}

void UUfAnimNotifyState_Hit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if(UUfSkillComponent* Skill = UUfSkillComponent::GetSkillComponent(MeshComp))
	{
	}
	// 치트 매니저 체크해서 충돌 표시가 true라면 DrawHitShape 호출
	// 치트매니저도 이쪽으로 와야하는가????
}

void UUfAnimNotifyState_Hit::DrawHitShape(UWorld* InWorld, const FUfHitShape& InHitShape)
{
	if(InWorld == nullptr)
		return;

	switch(InHitShape.ShapeType)
	{
	case EUfHitShapeType::Box:
		::DrawDebugPoint(InWorld, InHitShape.Location, 10, HitColor);
		::DrawDebugBox(InWorld, InHitShape.Location, InHitShape.GetBoxSize(), HitColor);
		break;
	case EUfHitShapeType::Fan:
		break;
	case EUfHitShapeType::Donut:
		break;
	default:
		break;
	}
}
