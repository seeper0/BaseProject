// Fill out your copyright notice in the Description page of Project Settings.


#include "CfSkillInputComponent.h"
#include "CfLogger.h"
#include "CfActionBase.h"
#include "CfActionComponent.h"
#include "CfSkillData.h"

void UCfSkillInputComponent::SetupComponent(class UCfActionComponent* InActionComponent, UEnhancedInputComponent* EnhancedInputComponent)
{
	ActionComponent = InActionComponent;
	InputSlotCache.Empty();
	for (const auto& SkillSlot : InputSlotMapping)
	{
		switch (SkillSlot.Key)
		{
		case ECfSkillKey::Move: // Moving
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Triggered, this, &UCfSkillInputComponent::Move);
			break;
		case ECfSkillKey::Look: // Looking
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Triggered, this, &UCfSkillInputComponent::Look);
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

ECfSkillKey UCfSkillInputComponent::GetSkillSlot(const FInputActionInstance& InputActionInstance) const
{
	const ECfSkillKey* SkillKey = InputSlotCache.Find(InputActionInstance.GetSourceAction());
	if(SkillKey == nullptr)
		return ECfSkillKey::None;

	return *SkillKey;
}

TArray<FName> UCfSkillInputComponent::FetchSkillsByInput(const ECfSkillKey SkillKey, const ETriggerEvent KeyEvent) const
{
	TArray<FName> FetchedSkills;
	const UDataTable* SkillTable = ActionComponent->GetSkillTable();
	const ECfSkillState SkillState = ActionComponent->GetSkillState();
	if(SkillTable == nullptr)
		return FetchedSkills;

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

void UCfSkillInputComponent::Move(const FInputActionValue& Value)
{
	ACharacter* OwnerChar = ActionComponent->GetOwnerChar();
	const AController* Controller = ActionComponent->GetController();
	const UCfActionBase* CurrentAction = ActionComponent->GetCurrentAction();
	
	if(CurrentAction && !CurrentAction->CanMoveDuring())
		return;

	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (OwnerChar && Controller)
	{
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
	}
}

void UCfSkillInputComponent::Look(const FInputActionValue& Value)
{
	ACharacter* OwnerChar = ActionComponent->GetOwnerChar();
	const AController* Controller = ActionComponent->GetController();
	
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (OwnerChar && Controller)
	{
		// add yaw and pitch input to controller
		OwnerChar->AddControllerYawInput(LookAxisVector.X);
		OwnerChar->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void UCfSkillInputComponent::OnPress(const FInputActionInstance& InputActionInstance)
{
	const ECfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == ECfSkillKey::None)
		return;

	// 키를 눌렀을때 다음에 뭐쓸지 여기서 결정하자. 결정을 바꾸면 로직이 복잡해진다.
	const TArray<FName> FetchedSkills = FetchSkillsByInput(SkillKey, ETriggerEvent::Started);
	const FCfSkillData* SkillData = ActionComponent->GetDesiredSkill(FetchedSkills);
	ActionComponent->InputSkill(SkillData);
}

void UCfSkillInputComponent::OnHold(const FInputActionInstance& InputActionInstance)
{
	const ECfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == ECfSkillKey::None)
		return;

	const UCfActionBase* CurrentAction = ActionComponent->GetCurrentAction();
	if(CurrentAction && CurrentAction->CanInputDuring()) // 차지 스킬일때는 딱히 아무것도 안해도 됨
	{
		// 키를 눌렀을때 다음에 뭐쓸지 여기서 결정하자. 결정을 바꾸면 로직이 복잡해진다.
		const TArray<FName> FetchedSkills = FetchSkillsByInput(SkillKey, ETriggerEvent::Ongoing);
		const FCfSkillData* SkillData = ActionComponent->GetDesiredSkill(FetchedSkills);
		ActionComponent->InputSkill(SkillData);
	}
}

void UCfSkillInputComponent::OnRelease(const FInputActionInstance& InputActionInstance)
{
	const ECfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == ECfSkillKey::None)
		return;

	UCfActionBase* CurrentAction = ActionComponent->GetCurrentAction();
	if(CurrentAction)
	{
		CurrentAction->ReleaseButton(SkillKey);
	}
}
