// Fill out your copyright notice in the Description page of Project Settings.


#include "CfStatComponent.h"

// Sets default values for this component's properties
UCfStatComponent::UCfStatComponent()
{
}

void UCfStatComponent::BeginPlay()
{
	Super::BeginPlay();

	TargetInfo = CreateWidget<UCfTargetInfoWidget>( GetWorld()->GetGameInstance(), TargetInfoWidgetClass, NAME_None );
	if(TargetInfo)
	{
		//TargetInfo->AddToViewport(static_cast<int32>(ECfZOrder::TargetInfo));
	}

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

	if(TargetInfo)
	{
		TargetInfo->Update(DamagedActor, TargetInfoHeight, HP, MaxHP);
	}
}

