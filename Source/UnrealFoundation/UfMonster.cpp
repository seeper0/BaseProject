// Fill out your copyright notice in the Description page of Project Settings.


#include "UfMonster.h"
#include "CfActionComponent.h"

// Sets default values
AUfMonster::AUfMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActionComponent = CreateDefaultSubobject<UCfActionComponent>(UCfActionComponent::ComponentName);
}

// Called when the game starts or when spawned
void AUfMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUfMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUfMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

