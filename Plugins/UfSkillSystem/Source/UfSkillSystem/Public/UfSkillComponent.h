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

	struct FSkillInfo
	{
		bool PreInput = false;
		const struct FUfSkillData* SkillData = nullptr;
	};

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
	EUfSkillState GetSkillState() const { return SkillState; }

private:
	EUfSkillKey GetSkillSlot(const FInputActionInstance& InputActionInstance) const;
	/** CurrentAction, SkillState, SkillKey, KeyEvent 조합으로 적절한 스킬을 찾는다. */
	TArray<FName> FetchSkillsByInput(const EUfSkillKey SkillKey, const ETriggerEvent KeyEvent) const;
	const FUfSkillData* GetDesiredSkill(const TArray<FName>& RowNames) const;

	/** Called for movement input */
	void Move(const FInputActionValue& InputActionValue);
	/** Called for looking input */
	void Look(const FInputActionValue& InputActionValue);
	void OnPress(const FInputActionInstance& InputActionInstance);
	void OnHold(const FInputActionInstance& InputActionInstance);
	void OnRelease(const FInputActionInstance& InputActionInstance);

	bool CanCancelSkill(const FUfSkillData* InSkillData) const;
	bool CanPlaySkill(const FUfSkillData* InSkillData) const;
	void InputSkill(const FUfSkillData* InSkillData);
	void PlaySkill(const FUfSkillData* InSkillData);
	void PlayAction(UAnimMontage* InMontage, const FUfSkillData* InSkillData);
	void TickAction();
	void ClearAction();

	UFUNCTION()
	void OnMontageEnd(UAnimMontage* Montage, bool bInterrupted);

	void ReserveSkill(const FUfSkillData* InSkillData);
	void TickInput();
	
	/** Skill Table */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* SkillTable = nullptr;

	/** Slot Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
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

	UPROPERTY()
	FName ReservedRowName;
};
