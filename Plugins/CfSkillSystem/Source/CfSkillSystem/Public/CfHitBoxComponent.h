// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CfCheatManager.h"
#include "Components/BoxComponent.h"
#include "CfHitBoxComponent.generated.h"

/**
 * 히트박스, 여러개 사용해서 관절에 붙일수 있게 개발
 */
UCLASS(ClassGroup = (CombatFoundation), meta = (BlueprintSpawnableComponent))
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
