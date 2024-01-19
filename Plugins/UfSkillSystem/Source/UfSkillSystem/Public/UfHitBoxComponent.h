// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "UfHitBoxComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Uf), meta = (BlueprintSpawnableComponent))
class UFSKILLSYSTEM_API UUfHitBoxComponent : public UBoxComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
