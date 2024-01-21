// Fill out your copyright notice in the Description page of Project Settings.


#include "CfAnimNotifyState_Hit.h"

#include "CfCheatManager.h"
#include "CfSkillComponent.h"
#include "GameFramework/Character.h"

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

		if(APlayerController* PC = Skill->GetWorld()->GetFirstPlayerController())
		{
			CheatManager = Cast<UCfCheatManager>(PC->CheatManager);
		}
	}
}

void UCfAnimNotifyState_Hit::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if(UCfSkillComponent* Skill = UCfSkillComponent::GetSkillComponent(MeshComp))
	{
		if(CheatManager && CheatManager->IsShowHitBox())
		{
			FTransform Transform = MeshComp->GetComponentToWorld(); //MeshComp->GetComponentToWorld(); //Skill->GetOwner()->ActorToWorld();// * MeshComp->GetRelativeTransform();
			FRotator Rotator = Transform.Rotator();
			Transform.SetRotation(FQuat(FRotator(Rotator.Pitch, Rotator.Yaw + 90, Rotator.Roll)));
			DrawHitShape(Skill->GetWorld(), HitShape, Transform);

			FVector StartPosition = Transform.GetLocation();
			Rotator.Yaw += 270;
			FVector Direction = Transform.GetRotation().GetForwardVector();
			Direction.Normalize();
			FVector EndPosition = StartPosition + Direction * 100;
			
			::DrawDebugPoint(Skill->GetWorld(), Transform.GetLocation(), 10, FColor::Red);
			::DrawDebugDirectionalArrow(Skill->GetWorld(), StartPosition, EndPosition, 10, FColor::Cyan);
		}

		if(IsHitSuccessful(Skill->GetWorld(), HitShape, MeshComp->GetComponentToWorld()))
		{
		}
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

void UCfAnimNotifyState_Hit::DrawHitShape(UWorld* InWorld, const FCfHitShape& InHitShape, const FTransform& ActorTransform)
{
	if(InWorld == nullptr)
		return;

	const FTransform Transform = ActorTransform * InHitShape.GetTransform();
	switch(InHitShape.ShapeType)
	{
	case ECfHitShapeType::Box:
		::DrawDebugPoint(InWorld, Transform.GetLocation(), 10, HitColor);
		::DrawDebugBox(InWorld, Transform.GetLocation(), InHitShape.GetBoxSize(), Transform.Rotator().Quaternion(), HitColor);
		break;
	case ECfHitShapeType::Fan:
		break;
	case ECfHitShapeType::Donut:
		break;
	default:
		break;
	}
}

bool UCfAnimNotifyState_Hit::IsHitSuccessful(UWorld* InWorld, const FCfHitShape& InHitShape, const FTransform& ActorTransform)
{
	if(InWorld == nullptr)
		return false;
	
	const FTransform Transform = ActorTransform * InHitShape.GetTransform();
	switch(InHitShape.ShapeType)
	{
	case ECfHitShapeType::Box:
		break;
	case ECfHitShapeType::Fan:
		break;
	case ECfHitShapeType::Donut:
		break;
	default:
		break;
	}

	return false;
}
