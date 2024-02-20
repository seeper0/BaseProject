// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CfOverlayComponent.h"
#include "CfOverlayInfoComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (CombatFoundation), meta = (BlueprintSpawnableComponent))
class CFSKILLSYSTEM_API UCfOverlayInfoComponent : public UCfOverlayComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void ShowWidget(bool bShow) override;
	void Update(AActor* DamagedActor, int32 HP, int32 MaxHP);

private:
	bool bHideDeferred = false;
};
