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
	UCfMarkingComponent* GetLockingComponent() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float LockingDistance = 1500.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float LockingAngle = 15.0f;
	
	UFUNCTION()
	class UCfMarkingComponent* FindLockingTarget() const;

	// 락온 기능
	// 락온 돌리기 기능
	// 에임, 캐릭터 선택 기능

	// 락온이면 캐릭터가 돌고 카메라 쫓아간다.
	// 락온이 아니면 에임 방향으로 공격한다.

	// 에임에서 HitBox를 추적하고 그 HitBox에서 가장 가까운 UCfMarkingComponent (TAG_Locking) 를 찾는다.

	// UCfMarkingComponent 에서 Tag 별로 렌더링하는 방법이 필요해보임
	// "Marker.Locking"
};
