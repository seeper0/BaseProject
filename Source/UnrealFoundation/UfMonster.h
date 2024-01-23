// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UfMonster.generated.h"

UCLASS()
class UNREALFOUNDATION_API AUfMonster : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillSystem, meta = (AllowPrivateAccess = "true"))
	class UCfActionComponent* ActionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillSystem, meta = (AllowPrivateAccess = "true"))
	class UCfStatComponent* StatComponent;

public:
	// Sets default values for this character's properties
	AUfMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE class UCfActionComponent* GetActionComponent() const { return ActionComponent; }
};
