// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "UfEngineSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UNREALFOUNDATION_API UUfEngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
