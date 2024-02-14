// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CfCheatManager.h"
#include "UfCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class UUfCheatManager : public UCfCheatManager
{
	GENERATED_BODY()
	
	UFUNCTION(Exec)
	void ChangeSpeed( int32 ChangeType );
};
