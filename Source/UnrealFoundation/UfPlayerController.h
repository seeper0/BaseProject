// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UfPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALFOUNDATION_API AUfPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	AUfPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
