// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "CfSkill.h"
#include "CfSkillInputComponent.generated.h"


UCLASS( ClassGroup=(CombatFoundation), meta=(BlueprintSpawnableComponent) )
class CFSKILLSYSTEM_API UCfSkillInputComponent : public UActorComponent
{
	GENERATED_BODY()

	struct FInputKey
	{
		ECfSkillKey SkillKey;
		ETriggerEvent KeyEvent;
		FVector InputDirection;
	};

public:
	inline static FName ComponentName = TEXT("SkillInputComponent");
	UCfSkillInputComponent();
	void SetupComponent(class UCfActionComponent* InActionComponent, UEnhancedInputComponent* EnhancedInputComponent);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FVector GetInputDirection(const FVector& InInputDirection, const ECfCardinalDirection DefaultDirection);
	ECfCardinalDirection GetCardinalDirection(const FVector& InInputDirection);

private:
	void TickInput(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);
	
	ECfSkillKey GetSkillSlot(const struct FInputActionInstance& InputActionInstance) const;
	/** CurrentAction, SkillState, SkillKey, KeyEvent 조합으로 적절한 스킬을 찾는다. */
	TArray<FName> FetchSkillsByInput(const ECfSkillKey SkillKey, const ETriggerEvent KeyEvent) const;

	/** Called for movement input */
	void BeginMove();
	void Move(const FInputActionValue& InputActionValue);
	void EndMove();
	/** Called for looking input */
	void Look(const FInputActionValue& InputActionValue);
	void OnPress(const FInputActionInstance& InputActionInstance);
	void OnHold(const FInputActionInstance& InputActionInstance);
	void OnRelease(const FInputActionInstance& InputActionInstance);
	// 기능
	void ToggleLockOn();
	bool HasLockingComponent() const;

	/** Slot Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TMap<ECfSkillKey, const UInputAction*> InputSlotMapping;

	UPROPERTY()
	TMap<const UInputAction*, ECfSkillKey> InputSlotCache;

	UPROPERTY()
	class UCfActionComponent* ActionComponent;

	bool bUseControllerDesiredRotation = false;

	TQueue<FInputKey> InputQueue;
	int32 InputQueueSize = 0; 
	double LastInputTime = 0.0; 

	FVector InputWorldDirection;
};

