// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "UfCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class UNREALFOUNDATION_API UUfCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
	UFUNCTION(Exec)
	void MoveSpeed( int32 ChangeType );
};
