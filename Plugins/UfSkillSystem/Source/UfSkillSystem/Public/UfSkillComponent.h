// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UfSkill.h"
#include "UfSkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UFSKILLSYSTEM_API UUfSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUfSkillComponent();
	static UUfSkillComponent* GetSkillComponent(USkeletalMeshComponent* MeshComp)
	{
		if(MeshComp && MeshComp->GetOwner())
		{
			return MeshComp->GetOwner()->GetComponentByClass<UUfSkillComponent>();
		}
		return nullptr;
	}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PlaySkill();
	void SetSkillState(ESkillState InSkillState);

private:
	UPROPERTY()
	ESkillState SkillState;
};
