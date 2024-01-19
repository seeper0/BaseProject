// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "UfSkill.h"
#include "UfAnimNotifyState_Hit.generated.h"

/**
 * 
 */
UCLASS()
class UFSKILLSYSTEM_API UUfAnimNotifyState_Hit : public UAnimNotifyState
{
	GENERATED_BODY()
	
	UUfAnimNotifyState_Hit(const FObjectInitializer& ObjectInitializer);
	virtual FString GetNotifyName_Implementation() const override { return TEXT("Hit"); }

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	const FUfHitShape& GetHitShape() const { return HitShape; }
	
	static void DrawHitShape(UWorld* InWorld, const FUfHitShape& InHitShape);
	static FColor HitColor;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FUfHitShape HitShape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FUfHitData HitData;

private:
};
