// Fill out your copyright notice in the Description page of Project Settings.


#include "CfHitBoxComponent.h"

void UCfHitBoxComponent::BeginPlay()
{
	Super::BeginPlay();

	SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}
