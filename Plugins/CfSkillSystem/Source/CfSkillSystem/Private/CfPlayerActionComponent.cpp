// Fill out your copyright notice in the Description page of Project Settings.


#include "CfPlayerActionComponent.h"
#include "CfLogger.h"
#include "CfActionBase.h"
#include "CfSkillData.h"

void UCfPlayerActionComponent::SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent)
{
	SkillSlotCache.Empty();
	for (const auto& SkillSlot : SkillSlotMapping)
	{
		switch (SkillSlot.Key)
		{
		case ECfSkillKey::Move: // Moving
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Triggered, this, &UCfPlayerActionComponent::Move);
			break;
		case ECfSkillKey::Look: // Looking
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Triggered, this, &UCfPlayerActionComponent::Look);
			break;
		default:
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Started, this, &UCfPlayerActionComponent::OnPress);
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Ongoing, this, &UCfPlayerActionComponent::OnHold);
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Completed, this, &UCfPlayerActionComponent::OnRelease);
			SkillSlotCache.Add(SkillSlot.Value, SkillSlot.Key);
			break;
		}
	}
}

ECfSkillKey UCfPlayerActionComponent::GetSkillSlot(const FInputActionInstance& InputActionInstance) const
{
	const ECfSkillKey* SkillKey = SkillSlotCache.Find(InputActionInstance.GetSourceAction());
	if(SkillKey == nullptr)
		return ECfSkillKey::None;

	return *SkillKey;
}

TArray<FName> UCfPlayerActionComponent::FetchSkillsByInput(const ECfSkillKey SkillKey, const ETriggerEvent KeyEvent) const
{
	TArray<FName> FetchedSkills;
	if(SkillTable == nullptr)
		return FetchedSkills;

	const TArray<FName> RowNames = SkillTable->GetRowNames();
	switch (SkillState)
	{
	case ECfSkillState::PreDelay:
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

void UCfPlayerActionComponent::Move(const FInputActionValue& Value)
{
	if(CurrentAction && !CurrentAction->CanMoveDuring())
		return;

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (OwnerChar && OwnerChar->Controller)
	{
		// find out which way is forward
		const FRotator Rotation = OwnerChar->Controller->GetControlRotation();
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

void UCfPlayerActionComponent::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (OwnerChar && OwnerChar->Controller)
	{
		// add yaw and pitch input to controller
		OwnerChar->AddControllerYawInput(LookAxisVector.X);
		OwnerChar->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void UCfPlayerActionComponent::OnPress(const FInputActionInstance& InputActionInstance)
{
	const ECfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == ECfSkillKey::None)
		return;

	// 키를 눌렀을때 다음에 뭐쓸지 여기서 결정하자. 결정을 바꾸면 로직이 복잡해진다.
	const TArray<FName> FetchedSkills = FetchSkillsByInput(SkillKey, ETriggerEvent::Started);
	const FCfSkillData* SkillData = GetDesiredSkill(FetchedSkills);
	InputSkill(SkillData);
}

void UCfPlayerActionComponent::OnHold(const FInputActionInstance& InputActionInstance)
{
	const ECfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == ECfSkillKey::None)
		return;

	if(CurrentAction->CanInputDuring()) // 차지 스킬일때는 딱히 아무것도 안해도 됨
	{
		// 키를 눌렀을때 다음에 뭐쓸지 여기서 결정하자. 결정을 바꾸면 로직이 복잡해진다.
		const TArray<FName> FetchedSkills = FetchSkillsByInput(SkillKey, ETriggerEvent::Ongoing);
		const FCfSkillData* SkillData = GetDesiredSkill(FetchedSkills);
		InputSkill(SkillData);
	}
}

void UCfPlayerActionComponent::OnRelease(const FInputActionInstance& InputActionInstance)
{
	const ECfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == ECfSkillKey::None)
		return;

	if(CurrentAction)
	{
		CurrentAction->OnButtonReleased(SkillKey);
	}
}
