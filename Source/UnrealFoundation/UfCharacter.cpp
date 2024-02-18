// Copyright Epic Games, Inc. All Rights Reserved.

#include "UfCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CfLogger.h"
#include "Actions/CfActionComponent.h"
#include "Anims/CfAnimInstance.h"
#include "CfSkillInputComponent.h"
#include "CfStatComponent.h"
#include "CfCameraBoomComponent.h"

//////////////////////////////////////////////////////////////////////////
// AUnrealFoundationCharacter

AUfCharacter::AUfCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false; // 그냥 폰을 돌린다. (루트모션보다 우선순위가 높다.)
	bUseControllerRotationRoll = false;

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->GravityScale = 1.0f;
	GetCharacterMovement()->MaxAcceleration = 2400.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	GetCharacterMovement()->BrakingFriction = 6.0f;
	GetCharacterMovement()->GroundFriction = 8.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1400.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<UCfCameraBoomComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	ActionComponent = CreateDefaultSubobject<UCfActionComponent>(UCfActionComponent::ComponentName);
	SkillInputComponent = CreateDefaultSubobject<UCfSkillInputComponent>(UCfSkillInputComponent::ComponentName);
	StatComponent = CreateDefaultSubobject<UCfStatComponent>(UCfStatComponent::ComponentName);

#if WITH_EDITORONLY_DATA
	const float HalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	InputArrow = CreateDefaultSubobject<UArrowComponent>(UCfAnimInstance::InputArrowName);
	InputArrow->SetupAttachment(RootComponent);
	InputArrow->SetHiddenInGame(false);
	InputArrow->SetVisibility(false);
	InputArrow->SetRelativeLocation(FVector(0, 0, -HalfHeight * 0.5f));
	InputArrow->SetArrowFColor(FColor::Yellow);
	InputArrow->SetArrowLength(0.0f);

	VelocityArrow = CreateDefaultSubobject<UArrowComponent>(UCfAnimInstance::VelocityArrowName);
	VelocityArrow->SetupAttachment(RootComponent);
	VelocityArrow->SetHiddenInGame(false);
	VelocityArrow->SetVisibility(false);
	VelocityArrow->SetRelativeLocation(FVector(0, 0, -HalfHeight * 0.5f));
	VelocityArrow->SetArrowFColor(FColor::Orange);
	VelocityArrow->SetArrowLength(0.0f);

	if (GetArrowComponent())
	{
		// GetArrowComponent()->SetHiddenInGame(false);
		// GetArrowComponent()->SetVisibility(true);
	}
#endif

}

void AUfCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUfCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Skill
		SkillInputComponent->SetupComponent(ActionComponent, EnhancedInputComponent);
	}
	else
	{
		CF_LOG(TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
