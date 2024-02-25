// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CfCharacterData.h"
#include "Actions/CfActionComponent.h"
#include "Components/ActorComponent.h"
#include "CfAvatarComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CFSKILLSYSTEM_API UCfAvatarComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	inline static FName ComponentName = TEXT("AvatarComponent");

	// Sets default values for this component's properties
	UCfAvatarComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	void Initialize(const FCfCharacterData* CharacterData);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	const struct FCfWeaponData* WeaponData1 = nullptr;
	const struct FCfWeaponData* WeaponData2 = nullptr;

	UPROPERTY()
	ACharacter* OwnerChar = nullptr;
	UPROPERTY()
	AActor* WeaponActor1 = nullptr;
	UPROPERTY()
	AActor* WeaponActor2 = nullptr;
};
