// Fill out your copyright notice in the Description page of Project Settings.


#include "CfStatComponent.h"
#include "HUD/CfHUD.h"
#include "HUD/CfOverlayInfoComponent.h"
#include "HUD/CfOverlayLockOnComponent.h"

// Sets default values for this component's properties
UCfStatComponent::UCfStatComponent()
{
}

void UCfStatComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UCfStatComponent::OnTakeDamage);
}

bool UCfStatComponent::IsCritical() const
{
	const float Rand = FMath::RandRange(0.0f, 1.0f);
	return Rand < CriticalRatio;
}

float UCfStatComponent::GetDamage(float DamageMultiplier, bool bIsCritical) const
{
	return GetBaseDamage() * DamageMultiplier * (bIsCritical ? CriticalDamageMultiplier : 1.0f);
}

void UCfStatComponent::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* InDamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	HP = FMath::Clamp(HP - Damage, 0.0f, MaxHP);

	if(UCfOverlayInfoComponent* Info = GetOwner()->GetComponentByClass<UCfOverlayInfoComponent>())
	{
		Info->ShowWidget(true);
		Info->Update(GetOwner(), HP, MaxHP);
		if(!IsLockOn())
		{
			Info->ShowWidget(false);
		}
	}
}

bool UCfStatComponent::IsLockOn() const
{
	if(const UCfOverlayLockOnComponent* Target = ACfHUD::GetLockingTarget(GetWorld()))
	{
		if(Target->GetOwner() == GetOwner())
		{
			return true;
		}
	}
	return false; 
}

