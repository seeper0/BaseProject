// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Components/ActorComponent.h"
#include "UfSkill.h"
#include "UfSkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UFSKILLSYSTEM_API UUfSkillComponent : public UActorComponent
{
	GENERATED_BODY()

	// struct Input
	// {
	// 	ESkillSlot Slot;
	// 	ETriggerEvent Event;
	// };
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

	void SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent);
	void PlaySkill();
	void SetSkillState(ESkillState InSkillState);

private:
	ESkillKey GetSkillSlot(const FInputActionInstance& InputActionInstance) const;
	const struct FUfSkillTable* FindSkill(const ESkillKey SkillKey) const;

	void OnPress(const FInputActionInstance& InputActionInstance);
	void OnTrigger(const FInputActionInstance& InputActionInstance);
	void OnRelease(const FInputActionInstance& InputActionInstance);


	/** Skill Table */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* SkillTable = nullptr;

	/** Slot Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<ESkillKey, const UInputAction*> SkillSlotMapping;

	UPROPERTY()
	TMap<const UInputAction*, ESkillKey> SkillSlotCache;
	
	UPROPERTY()
	ESkillState SkillState = ESkillState::None;

	UPROPERTY()
	ACharacter* OwnerChar = nullptr;

	// UPROPERTY()
	// TQueue<Input> InputQueue;
};
