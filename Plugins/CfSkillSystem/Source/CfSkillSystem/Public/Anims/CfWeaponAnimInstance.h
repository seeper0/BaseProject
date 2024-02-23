// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CfSkill.h"
#include "CfWeaponAnimInstance.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, hideCategories=AnimInstance, BlueprintType, Within=SkeletalMeshComponent)
class CFSKILLSYSTEM_API UCfWeaponAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe))
	class UCfAnimInstance* GetMainAnimInstance();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence", meta = (AllowPrivateAccess = "true"))
	UDataTable* LocomotionTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence", meta = (AllowPrivateAccess = "true"))
	FName CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence", meta = (AllowPrivateAccess = "true"))
	ECfWeaponType WeaponType = ECfWeaponType::None;

	const struct FCfAnimLocomotionData* LocomotionData = nullptr; 

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence", meta=(BlueprintThreadSafe))
	UAnimSequence* GetIdle() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence", meta=(BlueprintThreadSafe))
	UAnimSequence* GetWalk() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence|Walk", meta=(BlueprintThreadSafe))
	UAnimSequence* GetWalkFwd() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence|Walk", meta=(BlueprintThreadSafe))
	UAnimSequence* GetWalkFwdLeft() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence|Walk", meta=(BlueprintThreadSafe))
	UAnimSequence* GetWalkFwdRight() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence|Walk", meta=(BlueprintThreadSafe))
	UAnimSequence* GetWalkLeft() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence|Walk", meta=(BlueprintThreadSafe))
	UAnimSequence* GetWalkRight() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence|Walk", meta=(BlueprintThreadSafe))
	UAnimSequence* GetWalkBwd() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence|Walk", meta=(BlueprintThreadSafe))
	UAnimSequence* GetWalkBwdLeft() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence|Walk", meta=(BlueprintThreadSafe))
	UAnimSequence* GetWalkBwdRight() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence|Run", meta=(BlueprintThreadSafe))
	UAnimSequence* GetRunFwd() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence|Run", meta=(BlueprintThreadSafe))
	UAnimSequence* GetRunFwdLeft() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence|Run", meta=(BlueprintThreadSafe))
	UAnimSequence* GetRunFwdRight() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence|Run", meta=(BlueprintThreadSafe))
	UAnimSequence* GetRunLeft() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence|Run", meta=(BlueprintThreadSafe))
	UAnimSequence* GetRunRight() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence|Run", meta=(BlueprintThreadSafe))
	UAnimSequence* GetRunBwd() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence|Run", meta=(BlueprintThreadSafe))
	UAnimSequence* GetRunBwdLeft() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence|Run", meta=(BlueprintThreadSafe))
	UAnimSequence* GetRunBwdRight() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence|Jump", meta=(BlueprintThreadSafe))
	UAnimSequence* GetJumpStart() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence|Jump", meta=(BlueprintThreadSafe))
	UAnimSequence* GetJumpLoop() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sequence|Jump", meta=(BlueprintThreadSafe))
	UAnimSequence* GetJumpEnd() const;
};
