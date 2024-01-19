// Fill out your copyright notice in the Description page of Project Settings.


#include "UfHitBoxComponent.h"

void UUfHitBoxComponent::BeginPlay()
{
	Super::BeginPlay();

	SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}
