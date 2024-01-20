// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CfHUD.h"
#include "UfHUD.generated.h"

/**
 * 
 */
UCLASS()
class UNREALFOUNDATION_API AUfHUD : public ACfHUD
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
};
