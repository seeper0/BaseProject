// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "UfCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(config=Game)
class AUfCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillSystem, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillSystem, meta = (AllowPrivateAccess = "true"))
	class UCfActionComponent* ActionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillSystem, meta = (AllowPrivateAccess = "true"))
	class UCfSkillInputComponent* SkillInputComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillSystem, meta = (AllowPrivateAccess = "true"))
	class UCfStatComponent* StatComponent; 

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UArrowComponent* InputArrow;

	UPROPERTY()
	UArrowComponent* VelocityArrow;
#endif	
	
public:
	AUfCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns SkillComponent subobject **/
	FORCEINLINE class UCfActionComponent* GetActionComponent() const { return ActionComponent; }
};

