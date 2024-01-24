// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CfSkill.h"
#include "CfActionComponent.h"
#include "CfAnimNotifyState_Hit.generated.h"

/**
 * 
 */
UCLASS()
class CFSKILLSYSTEM_API UCfAnimNotifyState_Hit : public UAnimNotifyState
{
	GENERATED_BODY()
	
	UCfAnimNotifyState_Hit(const FObjectInitializer& ObjectInitializer);
	virtual FString GetNotifyName_Implementation() const override { return TEXT("Hit"); }

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

	TArray<ACharacter*> GetHitSuccessful(const FCfHitShape& InHitShape, const FTransform& ActorTransform) const;
public:
	inline static FColor HitColor = FColor(230, 175, 20, 255);
	const FCfHitShape& GetHitShape() const { return HitShape; }
	
	static void DrawHitShape(UWorld* InWorld, const FCfHitShape& InHitShape, const FTransform& ActorTransform);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCfHitShape HitShape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCfHitData HitData;

private:
	UPROPERTY()
	class UCfCheatManager* CheatManager;
	UPROPERTY()
	UCfActionComponent* Skill;
};
