// Fill out your copyright notice in the Description page of Project Settings.


#include "CfHitReactionComponent.h"
#include "CfLogger.h"
#include "CfSkill.h"

// Sets default values for this component's properties
UCfHitReactionComponent::UCfHitReactionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCfHitReactionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UCfHitReactionComponent::OnTakeDamage);
}


// Called every frame
void UCfHitReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCfHitReactionComponent::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* InDamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	const UCfDamageType* DamageType = Cast<UCfDamageType>(InDamageType);
	if(DamageType == nullptr)
		return;

	CF_LOG(TEXT("OnTakeDamage"));
}


