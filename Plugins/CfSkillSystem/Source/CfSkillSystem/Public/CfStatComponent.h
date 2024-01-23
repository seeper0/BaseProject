// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CfTargetInfoWidget.h"
#include "CfStatComponent.generated.h"


UCLASS( ClassGroup=(CombatFoundation), meta=(BlueprintSpawnableComponent) )
class CFSKILLSYSTEM_API UCfStatComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	UCfStatComponent();

	float GetBaseDamage() const { return Attack; }
	bool IsCritical() const;
	float GetDamage(float DamageMultiplier, bool bIsCritical) const;

protected:
	UFUNCTION()
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* InDamageType, AController* InstigatedBy, AActor* DamageCauser);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCfTargetInfoWidget> TargetInfoWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float TargetInfoHeight = 80;
	
	UPROPERTY()
	UCfTargetInfoWidget* TargetInfo; 
	
	int32 HP = 100;
	int32 MaxHP = 100;
	int32 Attack = 10;
	int32 Defense = 0;
	float MovementSpeed = 0;
	float CriticalRatio = 0.1f;
	float CriticalDamageMultiplier = 2.5f;
};
