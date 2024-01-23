// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CfTargetInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class CFSKILLSYSTEM_API UCfTargetInfoWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HpBar;

public:
	void Update(AActor* DamagedActor, float TargetInfoHeight, int32 HP, int32 MaxHP);
};
