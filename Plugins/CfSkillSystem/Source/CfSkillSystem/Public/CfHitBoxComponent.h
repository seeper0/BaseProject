// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "CfHitBoxComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Cf), meta = (BlueprintSpawnableComponent))
class CFSKILLSYSTEM_API UCfHitBoxComponent : public UBoxComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
