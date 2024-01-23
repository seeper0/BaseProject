// Fill out your copyright notice in the Description page of Project Settings.


#include "CfAnimNotifyState_Hit.h"

#include "CfCheatManager.h"
#include "CfActionComponent.h"
#include "CfLogger.h"
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

	Skill = UCfActionComponent::GetActionComponent(MeshComp);
	if(Skill)
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

	if(Skill)
	{
		if(CheatManager && CheatManager->IsShowHitBox())
		{
			CF_TODO("아직 계산이 잘못되었다.");
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

		constexpr float BaseDamage = 10.0f;
		const float Damage = BaseDamage * HitData.DamageMultiplier;
		AController* EventInstigator = Skill->GetController();
		ACharacter* DamageCauser = Skill->GetOwnerChar();
		FCfDamageEvent DamageEvent;
		DamageEvent.HitData = HitData;
		DamageEvent.DamageTypeClass = DamageTypeClass;

		CF_TODO("Take 에서 Damage 애니메이션, KnockBack, Down, Airborne 등을 구현해야한다.");
		TArray<ACharacter*> List = GetHitSuccessful(HitShape, MeshComp->GetComponentToWorld());
		for(ACharacter* Char : List)
		{
			//if(Char->ShouldTakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser))
			Char->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
		}
	}
	//MeshComp->GetWorld()->WorldType : EWorldType::Type::EditorPreview
}

void UCfAnimNotifyState_Hit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if(Skill)
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

TArray<ACharacter*> UCfAnimNotifyState_Hit::GetHitSuccessful(const FCfHitShape& InHitShape, const FTransform& ActorTransform) const
{
	TArray<ACharacter*> List;
	if(Skill == nullptr)
		return List;

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Skill->GetOwner());
	bool bOverlap = false;

	const FTransform Transform = ActorTransform * InHitShape.GetTransform();
	switch(InHitShape.ShapeType)
	{
	case ECfHitShapeType::Box:
		bOverlap = Skill->GetWorld()->OverlapMultiByObjectType(Overlaps, Transform.GetLocation(), Transform.GetRotation(), ObjectParams, FCollisionShape::MakeBox(InHitShape.GetBoxSize()), CollisionParams);
		break;
	case ECfHitShapeType::Fan:
		break;
	case ECfHitShapeType::Donut:
		break;
	default:
		break;
	}

	if(bOverlap)
	{
		for(const FOverlapResult& Overlap : Overlaps)
		{
			if(Overlap.GetComponent())
			{
				if(ACharacter* Char = Cast<ACharacter>(Overlap.GetComponent()->GetOwner()))
				{
					if(!Skill->HasActorInHitList(Char))
					{
						Skill->PushHitActorList(Char);
						List.AddUnique(Char);
					}
				}
			}
		}
	}

	return List;
}
