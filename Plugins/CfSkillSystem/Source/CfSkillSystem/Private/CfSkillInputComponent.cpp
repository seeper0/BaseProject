// Fill out your copyright notice in the Description page of Project Settings.


#include "CfSkillInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CfLogger.h"
#include "Actions/CfActionBase.h"
#include "Actions/CfActionComponent.h"
#include "CfCameraBoomComponent.h"
#include "CfSkillAsset.h"
#include "CfSkillData.h"
#include "CfUtil.h"

UCfSkillInputComponent::UCfSkillInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCfSkillInputComponent::SetupComponent(class UCfActionComponent* InActionComponent, UEnhancedInputComponent* EnhancedInputComponent)
{
	check(InActionComponent);
	check(EnhancedInputComponent);

	ActionComponent = InActionComponent;
	InputSlotCache.Empty();
	for (const auto& SkillSlot : InputSlotMapping)
	{
		switch (SkillSlot.Key)
		{
		case ECfSkillKey::Move: // Moving
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Started, this, &UCfSkillInputComponent::BeginMove);
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Triggered, this, &UCfSkillInputComponent::Move);
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Completed, this, &UCfSkillInputComponent::EndMove);
			break;
		case ECfSkillKey::Look: // Looking
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Triggered, this, &UCfSkillInputComponent::Look);
			break;
		case ECfSkillKey::LockOn:
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Started, this, &UCfSkillInputComponent::ToggleLockOn);
			break;
		default:
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Started, this, &UCfSkillInputComponent::OnPress);
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Ongoing, this, &UCfSkillInputComponent::OnHold);
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Completed, this, &UCfSkillInputComponent::OnRelease);
			InputSlotCache.Add(SkillSlot.Value, SkillSlot.Key);
			break;
		}
	}
}

void UCfSkillInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ACharacter* OwnerChar = ActionComponent->GetOwnerChar();
	const UCfActionBase* CurrentAction = ActionComponent->GetCurrentAction();
	//::DrawDebugDirectionalArrow(OwnerChar->GetWorld(), OwnerChar->GetActorLocation(), OwnerChar->GetActorLocation() + InputWorldDirection * 200, 100, FColor::Red);  

	if(UCharacterMovementComponent* MovementComponent = OwnerChar ? OwnerChar->GetCharacterMovement() : nullptr)
	{
		MovementComponent->bUseControllerDesiredRotation = bUseControllerDesiredRotation;
	}

	TickInput(DeltaTime, TickType, ThisTickFunction);
}

FVector UCfSkillInputComponent::GetInputDirection(const FVector& InInputDirection, const ECfCardinalDirection DefaultDirection)
{
	const AController* Controller = ActionComponent->GetController();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	if(InInputDirection.IsNearlyZero())
	{
		switch (DefaultDirection)
		{
		case ECfCardinalDirection::Forward:
			return FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		case ECfCardinalDirection::Backward:
			return FRotationMatrix(YawRotation + FRotator(0, 180, 0)).GetUnitAxis(EAxis::X);
		case ECfCardinalDirection::Right:
			return FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		case ECfCardinalDirection::Left:
			return FRotationMatrix(YawRotation + FRotator(0, 180, 0)).GetUnitAxis(EAxis::Y);
		}
		return FVector::Zero();
	}
	return InInputDirection;
}

ECfCardinalDirection UCfSkillInputComponent::GetCardinalDirection(const FVector& InInputDirection)
{
	const AController* Controller = ActionComponent->GetController();
	const FVector ControllerDirection = Controller->GetControlRotation().Vector();
	const float AngleBetween = FMath::RadiansToDegrees(acosf(FVector::DotProduct(ControllerDirection, InInputDirection)));

	if (AngleBetween < 45.0f)
	{
		return ECfCardinalDirection::Forward;
	}
	else if (AngleBetween > 135.0f)
	{
		return ECfCardinalDirection::Backward;
	}

	// 왼쪽 또는 오른쪽을 결정하기 위해 외적을 사용합니다.
	const FVector CrossProduct = FVector::CrossProduct(ControllerDirection, InInputDirection);
	if (CrossProduct.Z > 0)
	{
		return ECfCardinalDirection::Right;
	}
	return ECfCardinalDirection::Left;
}

void UCfSkillInputComponent::TickInput(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	check(ActionComponent);
	while(!InputQueue.IsEmpty() && !ActionComponent->IsReservedNext())
	{
		const FInputKey* Peek = InputQueue.Peek();
		const bool Pressed = Peek ? Peek->KeyEvent == ETriggerEvent::Started : false;
		if(Pressed)
		{
			FInputKey InputKey;
			InputQueue.Dequeue(InputKey);

			const TArray<FName> FetchedSkills = FetchSkillsByInput(InputKey.SkillKey, InputKey.KeyEvent);
			const FCfSkillData* SkillData = ActionComponent->GetDesiredSkill(FetchedSkills, InputKey.InputDirection);
			ActionComponent->InputSkill(SkillData, InputKey.InputDirection);
		}

		const bool Released = Peek ? Peek->KeyEvent == ETriggerEvent::Completed : false;
		if(Released)
		{
			FInputKey InputKey;
			InputQueue.Dequeue(InputKey);
			ActionComponent->ReleaseSkill(InputKey.SkillKey);
		}
	}
}


ECfSkillKey UCfSkillInputComponent::GetSkillSlot(const FInputActionInstance& InputActionInstance) const
{
	const ECfSkillKey* SkillKey = InputSlotCache.Find(InputActionInstance.GetSourceAction());
	if(SkillKey == nullptr)
		return ECfSkillKey::None;

	return *SkillKey;
}

TArray<FName> UCfSkillInputComponent::FetchSkillsByInput(const ECfSkillKey SkillKey, const ETriggerEvent KeyEvent) const
{
	check(ActionComponent);
	
	TArray<FName> FetchedSkills;
	const UDataTable* SkillTable = UCfSkillAsset::GetSkillTable();
	check(SkillTable);
	const ECfSkillState SkillState = ActionComponent->GetSkillState();

	const TArray<FName> RowNames = SkillTable->GetRowNames();
	switch (SkillState)
	{
	case ECfSkillState::NoInput:
		break;
	case ECfSkillState::None:
	case ECfSkillState::PreInput:
	case ECfSkillState::CanCancel:
	case ECfSkillState::End:
	case ECfSkillState::Over:
		{
			for (const FName RowName : RowNames)
			{
				const FCfSkillData* RowData = SkillTable->FindRow<FCfSkillData>(RowName, CF_FUNCTION);
				if(RowData == nullptr)
					continue;

				if(RowData->CharacterName != ActionComponent->GetCharacterName())
					continue;

				if(RowData->WeaponType != ActionComponent->GetWeaponType())
					continue;

				if(RowData->InputKey != SkillKey) // 입력키가 맞을 경우만 체크
					continue;

				if(KeyEvent == ETriggerEvent::Started || // 첫입력일 경우
					(KeyEvent == ETriggerEvent::Ongoing && RowData->InputType == ECfInputType::AutoRapid) ) // 오토콤보 설정이 있다면 누르고 있을때도 가능 
				{
					FetchedSkills.Add(RowName);
				}
			}
		}
		break;
	}
	
	return FetchedSkills;
}

void UCfSkillInputComponent::BeginMove()
{
	bUseControllerDesiredRotation = true;
	InputWorldDirection = FVector::ZeroVector;
}

void UCfSkillInputComponent::Move(const FInputActionValue& Value)
{
	check(ActionComponent);
	if(ActionComponent->IsEndSkill()) ActionComponent->StopSkill();

	ACharacter* OwnerChar = ActionComponent->GetOwnerChar();
	const AController* Controller = ActionComponent->GetController();
	const UCfActionBase* CurrentAction = ActionComponent->GetCurrentAction();

	check(OwnerChar);
	check(Controller);

	if(CurrentAction && !CurrentAction->CanMoveDuring())
		return;

	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	OwnerChar->AddMovementInput(ForwardDirection, MovementVector.Y);
	OwnerChar->AddMovementInput(RightDirection, MovementVector.X);

	InputWorldDirection = (ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X).GetSafeNormal();
}

void UCfSkillInputComponent::EndMove()
{
	bUseControllerDesiredRotation = false;
	InputWorldDirection = FVector::ZeroVector;
}

void UCfSkillInputComponent::Look(const FInputActionValue& Value)
{
	check(ActionComponent);
	ACharacter* OwnerChar = ActionComponent->GetOwnerChar();
	const AController* Controller = ActionComponent->GetController();

	check(OwnerChar);
	check(Controller);

	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	// add yaw and pitch input to controller
	if(HasLockingComponent() == false)
	{
		OwnerChar->AddControllerYawInput(LookAxisVector.X);
		OwnerChar->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void UCfSkillInputComponent::OnPress(const FInputActionInstance& InputActionInstance)
{
	const ECfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == ECfSkillKey::None)
		return;

	if(GetWorld()->GetTimeSeconds() - LastInputTime > 0.2f || InputQueueSize > 10)
	{
		InputQueueSize = 0;
		InputQueue.Empty();
	}

	//CF_LOG(TEXT("%s"), *FCfUtil::GetEnumString(SkillKey));
	InputQueue.Enqueue({SkillKey, ETriggerEvent::Started, InputWorldDirection});
	++InputQueueSize;
	LastInputTime = GetWorld()->GetTimeSeconds();

	// // 키를 눌렀을때 다음에 뭐쓸지 여기서 결정하자. 결정을 바꾸면 로직이 복잡해진다.
	// const TArray<FName> FetchedSkills = FetchSkillsByInput(SkillKey, ETriggerEvent::Started);
	// const FCfSkillData* SkillData = ActionComponent->GetDesiredSkill(FetchedSkills);
	// ActionComponent->InputSkill(SkillData, ETriggerEvent::Started);
}

void UCfSkillInputComponent::OnHold(const FInputActionInstance& InputActionInstance)
{
	const ECfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == ECfSkillKey::None)
		return;

	if(GetWorld()->GetTimeSeconds() - LastInputTime > 0.2f || InputQueueSize > 10)
	{
		InputQueueSize = 0;
		InputQueue.Empty();
	}

	const UCfActionBase* CurrentAction = ActionComponent->GetCurrentAction();
	if(CurrentAction && CurrentAction->CanInputAutoRapid()) // 연타라면 입력된것 처럼 해준다.
	{
		InputQueue.Enqueue({SkillKey, ETriggerEvent::Started, InputWorldDirection});
		++InputQueueSize;
		LastInputTime = GetWorld()->GetTimeSeconds();

		// // 키를 눌렀을때 다음에 뭐쓸지 여기서 결정하자. 결정을 바꾸면 로직이 복잡해진다.
		// const TArray<FName> FetchedSkills = FetchSkillsByInput(SkillKey, ETriggerEvent::Ongoing);
		// const FCfSkillData* SkillData = ActionComponent->GetDesiredSkill(FetchedSkills);
		// ActionComponent->InputSkill(SkillData, ETriggerEvent::Ongoing);
	}
}

void UCfSkillInputComponent::OnRelease(const FInputActionInstance& InputActionInstance)
{
	const ECfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == ECfSkillKey::None)
		return;

	if(GetWorld()->GetTimeSeconds() - LastInputTime > 0.2f || InputQueueSize > 10)
	{
		InputQueueSize = 0;
		InputQueue.Empty();
	}

	InputQueue.Enqueue({SkillKey, ETriggerEvent::Completed});
	++InputQueueSize;
	LastInputTime = GetWorld()->GetTimeSeconds();
	//ActionComponent->ReleaseSkill(SkillKey);
}

void UCfSkillInputComponent::ToggleLockOn()
{
	check(ActionComponent);

	const ACharacter* OwnerChar = ActionComponent->GetOwnerChar();
	check(OwnerChar);

	if(UCfCameraBoomComponent* Camera = OwnerChar->GetComponentByClass<UCfCameraBoomComponent>())
	{
		Camera->ToggleLockOn();
	}
}

bool UCfSkillInputComponent::HasLockingComponent() const
{
	if(UCfCameraBoomComponent::GetLockingComponent(GetWorld()))
		return true;

	return false;
}

