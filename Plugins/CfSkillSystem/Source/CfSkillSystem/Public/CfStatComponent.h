// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CfStatComponent.generated.h"


/**
 * Stat을 관리함
 */
UCLASS( ClassGroup=(CombatFoundation), meta=(BlueprintSpawnableComponent) )
class CFSKILLSYSTEM_API UCfStatComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	inline static FName ComponentName = TEXT("StatComponent");
	UCfStatComponent();

	float GetBaseDamage() const { return Attack; }
	bool IsCritical() const;
	float GetDamage(float DamageMultiplier, bool bIsCritical) const;

protected:
	UFUNCTION()
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* InDamageType, AController* InstigatedBy, AActor* DamageCauser);
	// Owner가 이미 LockOn 되어있다.
	UFUNCTION()
	bool IsLockOn() const;

private:
	int32 HP = 100;
	int32 MaxHP = 100;
	int32 Attack = 10;
	int32 Defense = 0;
	float WalkSpeed = 0;
	float SprintSpeed = 0;
	float CriticalRatio = 0.1f;
	float CriticalDamageMultiplier = 2.5f;
};
