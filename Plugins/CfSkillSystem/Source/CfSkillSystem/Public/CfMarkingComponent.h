// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "CfMarkingComponent.generated.h"

UCLASS(ClassGroup = (CombatFoundation), meta = (BlueprintSpawnableComponent))
class CFSKILLSYSTEM_API UCfMarkingComponent : public USphereComponent
{
	GENERATED_BODY()

public:	
	inline static FName ComponentName = TEXT("MarkingComponent");
	inline static FName TAG_Locking = TEXT("Marker.Locking");
	inline static FName TAG_Hp = TEXT("Marker.Hp");
	inline static FName TAG_Name = TEXT("Marker.Name");

	UCfMarkingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
