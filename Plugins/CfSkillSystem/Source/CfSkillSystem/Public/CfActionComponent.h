// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "CfSkill.h"
#include "GameFramework/Character.h"
#include "CfActionComponent.generated.h"


UCLASS( ClassGroup=(Cf), meta=(BlueprintSpawnableComponent) )
class CFSKILLSYSTEM_API UCfActionComponent : public UActorComponent
{
	GENERATED_BODY()

	struct FSkillInfo
	{
		bool PreInput = false;
		const struct FCfSkillData* SkillData = nullptr;
	};

public:	
	// Sets default values for this component's properties
	UCfActionComponent();
	inline static FName ComponentName = TEXT("SkillComponent");
	static UCfActionComponent* GetSkillComponent(const AActor* Actor);
	static UCfActionComponent* GetSkillComponent(const UActorComponent* MeshComp);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent);
	void SetSkillState(ECfSkillState InSkillState);
	ECfSkillState GetSkillState() const { return SkillState; }
	ACharacter* GetOwnerChar() const { return OwnerChar; }
	AController* GetController() const { return OwnerChar ? OwnerChar->Controller : nullptr; }

private:
	ECfSkillKey GetSkillSlot(const struct FInputActionInstance& InputActionInstance) const;
	/** CurrentAction, SkillState, SkillKey, KeyEvent 조합으로 적절한 스킬을 찾는다. */
	TArray<FName> FetchSkillsByInput(const ECfSkillKey SkillKey, const ETriggerEvent KeyEvent) const;
	const FCfSkillData* GetDesiredSkill(const TArray<FName>& RowNames) const;

	/** Called for movement input */
	void Move(const FInputActionValue& InputActionValue);
	/** Called for looking input */
	void Look(const FInputActionValue& InputActionValue);
	void OnPress(const FInputActionInstance& InputActionInstance);
	void OnHold(const FInputActionInstance& InputActionInstance);
	void OnRelease(const FInputActionInstance& InputActionInstance);

	bool CanCancelSkill(const FCfSkillData* InSkillData) const;
	bool CanPlaySkill(const FCfSkillData* InSkillData) const;
	void InputSkill(const FCfSkillData* InSkillData);
	void PlaySkill(const FCfSkillData* InSkillData);
	void PlayAction(UAnimMontage* InMontage, const FCfSkillData* InSkillData);
	void TickAction();
	void ClearAction();

	UFUNCTION()
	void OnMontageEnd(UAnimMontage* Montage, bool bInterrupted);

	void ReserveSkill(const FCfSkillData* InSkillData);
	void TickInput();
	
	/** Skill Table */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* SkillTable = nullptr;

	/** Slot Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TMap<ECfSkillKey, const UInputAction*> SkillSlotMapping;

	UPROPERTY()
	TMap<const UInputAction*, ECfSkillKey> SkillSlotCache;
	
	UPROPERTY()
	ECfSkillState SkillState = ECfSkillState::None;

	UPROPERTY()
	ACharacter* OwnerChar = nullptr;

	UPROPERTY()
	UAnimInstance* AnimInstance = nullptr; 

	UPROPERTY()
	class UCfActionBase* CurrentAction = nullptr;

	UPROPERTY()
	FName ReservedRowName;

#pragma region HitList
public:
	bool HasActorInHitList(const ACharacter* InVictim) const;
	void PushHitActorList(ACharacter* InVictim);
	void ClearHitActorList();

private:
	UPROPERTY()
	TSet<ACharacter*> HitActorList;

#pragma endregion
};
