// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "CfCameraBoomComponent.generated.h"

/**
 * 
 */
UCLASS()
class CFSKILLSYSTEM_API UCfCameraBoomComponent : public USpringArmComponent
{
	GENERATED_BODY()

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void ToggleLockOn();
	// 현재 락킹된 컴포넌트를 가져온다.
	static class UCfOverlayLockOnComponent* GetLockingComponent(UWorld* World);
	// 락온될 대상에서 가장 가까운 락온 컴포넌트를 찾는다.
	static class UCfOverlayLockOnComponent* FindLockingTarget(AActor* Owner, float LockingDistance, float LockingAngle = 15.0f);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float LockingDistance = 1500.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float LockingAngle = 15.0f;
};
