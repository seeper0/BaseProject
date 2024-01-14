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
	void SetSkillState(EUfSkillState InSkillState);

private:
	EUfSkillKey GetSkillSlot(const FInputActionInstance& InputActionInstance) const;
	const struct FUfSkillData* FindSkill(const EUfSkillKey SkillKey) const;
	const FUfSkillData* FindChainSkill(EUfSkillKey SkillKey) const;

	void OnPress(const FInputActionInstance& InputActionInstance);
	void OnTrigger(const FInputActionInstance& InputActionInstance);
	void OnRelease(const FInputActionInstance& InputActionInstance);

	bool CanCancelSkill(EUfSkillKey SkillKey, ETriggerEvent Started) const;
	bool CanPlaySkill(const FUfSkillData* Skill) const;
	void PlaySkill(EUfSkillKey SkillKey, ETriggerEvent Started);
	void PlayAction(UAnimMontage* InMontage, const FUfSkillData* Skill /* 임시 */);
	void TickAction();
	void ClearAction();

	UFUNCTION()
	void OnMontageEnd(UAnimMontage* Montage, bool bInterrupted);

	void QueueSkill(EUfSkillKey SkillKey, ETriggerEvent Started);
	void TickInput();
	void ProcessPreInputKey(const FUfInput& Input);
	
	/** Skill Table */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* SkillTable = nullptr;

	/** Slot Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<EUfSkillKey, const UInputAction*> SkillSlotMapping;

	UPROPERTY()
	TMap<const UInputAction*, EUfSkillKey> SkillSlotCache;
	
	UPROPERTY()
	EUfSkillState SkillState = EUfSkillState::None;

	UPROPERTY()
	ACharacter* OwnerChar = nullptr;

	UPROPERTY()
	UAnimInstance* AnimInstance = nullptr; 

	UPROPERTY()
	class UUfActionBase* CurrentAction = nullptr;

	TQueue<FUfInput> InputQueue;
};
