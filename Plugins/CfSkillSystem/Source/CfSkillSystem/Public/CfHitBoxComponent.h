// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CfCheatManager.h"
#include "Components/BoxComponent.h"
#include "CfHitBoxComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Cf), meta = (BlueprintSpawnableComponent))
class CFSKILLSYSTEM_API UCfHitBoxComponent : public UBoxComponent
{
	GENERATED_BODY()
public:
	inline static FName ComponentName = TEXT("HitBoxComponent");
	UCfHitBoxComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
private:
	UPROPERTY()
	UCfCheatManager* CheatManager = nullptr;
};
