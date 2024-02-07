// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CfWeaponAnimInstance.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, hideCategories=AnimInstance, BlueprintType, Within=SkeletalMeshComponent)
class CFANIMATIONSYSTEM_API UCfWeaponAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe))
	class UCfAnimInstance* GetMainAnimInstance();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_Idle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_WalkFwd;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_WalkFwdLeft;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_WalkFwdRight;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_WalkLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_WalkRight;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_WalkBwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_WalkBwdLeft;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Walk", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_WalkBwdRight;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_RunFwd;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_RunFwdLeft;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_RunFwdRight;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_RunLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_RunRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_RunBwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_RunBwdLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Run", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* As_RunBwdRight;
};
