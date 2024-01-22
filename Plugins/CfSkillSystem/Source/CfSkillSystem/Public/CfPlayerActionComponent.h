// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "CfActionComponent.h"
#include "CfPlayerActionComponent.generated.h"

/**
 * 
 */
UCLASS()
class CFSKILLSYSTEM_API UCfPlayerActionComponent : public UCfActionComponent
{
	GENERATED_BODY()

public:
	void SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent);

private:
	ECfSkillKey GetSkillSlot(const struct FInputActionInstance& InputActionInstance) const;
	/** CurrentAction, SkillState, SkillKey, KeyEvent 조합으로 적절한 스킬을 찾는다. */
	TArray<FName> FetchSkillsByInput(const ECfSkillKey SkillKey, const ETriggerEvent KeyEvent) const;

	/** Called for movement input */
	void Move(const FInputActionValue& InputActionValue);
	/** Called for looking input */
	void Look(const FInputActionValue& InputActionValue);
	void OnPress(const FInputActionInstance& InputActionInstance);
	void OnHold(const FInputActionInstance& InputActionInstance);
	void OnRelease(const FInputActionInstance& InputActionInstance);

public:
	/** Slot Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TMap<ECfSkillKey, const UInputAction*> SkillSlotMapping;

	UPROPERTY()
	TMap<const UInputAction*, ECfSkillKey> SkillSlotCache;
};
