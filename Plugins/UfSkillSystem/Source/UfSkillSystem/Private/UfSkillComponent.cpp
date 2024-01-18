// Fill out your copyright notice in the Description page of Project Settings.


#include "UfSkillComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "UfActionBase.h"
#include "UfLogger.h"
#include "UfSkillData.h"
#include "UfUtil.h"

// Sets default values for this component's properties
UUfSkillComponent::UUfSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UUfSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerChar = Cast<ACharacter>(GetOwner());
	// if(OwnerChar && OwnerChar->GetMesh())
	// {
	// 	AnimInstance = OwnerChar->GetMesh()->GetAnimInstance();
	// 	if(AnimInstance)
	// 	{
	// 		AnimInstance->OnMontageEnded.AddDynamic(this, &UUfSkillComponent::OnMontageEnd);
	// 	}
	// }
}


// Called every frame
void UUfSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//InputQueue.Empty();

	TickInput();
	TickAction();
}

void UUfSkillComponent::SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent)
{
	SkillSlotCache.Empty();
	for (const auto& SkillSlot : SkillSlotMapping)
	{
		switch (SkillSlot.Key)
		{
		case EUfSkillKey::Move: // Moving
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Triggered, this, &UUfSkillComponent::Move);
			break;
		case EUfSkillKey::Look: // Looking
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Triggered, this, &UUfSkillComponent::Look);
			break;
		default:
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Started, this, &UUfSkillComponent::OnPress);
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Ongoing, this, &UUfSkillComponent::OnHold);
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Completed, this, &UUfSkillComponent::OnRelease);
			SkillSlotCache.Add(SkillSlot.Value, SkillSlot.Key);
			break;
		}
	}
}

void UUfSkillComponent::SetSkillState(EUfSkillState InSkillState)
{
	SkillState = InSkillState;
	// if(Action)
	// 	Action->SetSkillState(InSkillState);

	// 선입력으로 예약된 스킬을 발동한다.
	if(SkillTable && SkillState == EUfSkillState::CanCancel)
	{
		if(ReservedRowName != NAME_None)
		{
			const FUfSkillData* SkillData = SkillTable->FindRow<FUfSkillData>(ReservedRowName, UF_FUNCTION);
			if(CanCancelSkill(SkillData))
			{
				PlaySkill(SkillData);
			}
			ReservedRowName = NAME_None;
		}
	}
}

EUfSkillKey UUfSkillComponent::GetSkillSlot(const FInputActionInstance& InputActionInstance) const
{
	const EUfSkillKey* SkillKey = SkillSlotCache.Find(InputActionInstance.GetSourceAction());
	if(SkillKey == nullptr)
		return EUfSkillKey::None;

	return *SkillKey;
}

TArray<FName> UUfSkillComponent::FetchSkillsByInput(const EUfSkillKey SkillKey, const ETriggerEvent KeyEvent) const
{
	TArray<FName> FetchedSkills;
	if(SkillTable == nullptr)
		return FetchedSkills;

	const TArray<FName> RowNames = SkillTable->GetRowNames();
	switch (SkillState)
	{
	case EUfSkillState::PreDelay:
		break;
	case EUfSkillState::None:
	case EUfSkillState::PreInput:
	case EUfSkillState::CanCancel:
	case EUfSkillState::End:
	case EUfSkillState::Over:
		{
			for (const FName RowName : RowNames)
			{
				const FUfSkillData* RowData = SkillTable->FindRow<FUfSkillData>(RowName, UF_FUNCTION);
				if(RowData == nullptr)
					continue;

				if(RowData->Key != SkillKey) // 입력키가 맞을 경우만 체크
					continue;

				if(KeyEvent == ETriggerEvent::Started || // 첫입력일 경우
					(KeyEvent == ETriggerEvent::Ongoing && RowData->InputType == EUfInputType::AutoRapid) ) // 오토콤보 설정이 있다면 누르고 있을때도 가능 
				{
					FetchedSkills.Add(RowName);
				}
			}
		}
		break;
	}
	
	return FetchedSkills;
}

const FUfSkillData* UUfSkillComponent::GetDesiredSkill(const TArray<FName>& RowNames) const
{
	// 체인 조건이 있다면 먼저 찾는다. (일단 현재 실행되는 스킬이 있어야함)
	if(CurrentAction)
	{
		const FName CurrentName = CurrentAction->GetSkillTable()->RowName;
		for (const FName RowName : RowNames)
		{
			const FUfSkillData* RowData = SkillTable->FindRow<FUfSkillData>(RowName, UF_FUNCTION);
			if(RowData->CanChain(CurrentName))
			{
				return  RowData;
			}
		}
	}
	// 체인조건이 없다면 평타 조건 찾는다.
	for (const FName RowName : RowNames)
	{
		const FUfSkillData* RowData = SkillTable->FindRow<FUfSkillData>(RowName, UF_FUNCTION);
		if(RowData->NotChain())
		{
			return  RowData;
		}
	}

	// 스킬이 많아지면 Priority 로... Chain 이 먼저, Cancel기 다음, 마지막 평타...

	return nullptr;
}

void UUfSkillComponent::Move(const FInputActionValue& Value)
{
	if(CurrentAction && !CurrentAction->CanMove())
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

void UUfSkillComponent::Look(const FInputActionValue& Value)
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

void UUfSkillComponent::OnPress(const FInputActionInstance& InputActionInstance)
{
	const EUfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == EUfSkillKey::None)
		return;

	// 키를 눌렀을때 다음에 뭐쓸지 여기서 결정하자. 결정을 바꾸면 로직이 복잡해진다.
	const TArray<FName> FetchedSkills = FetchSkillsByInput(SkillKey, ETriggerEvent::Started);
	const FUfSkillData* SkillData = GetDesiredSkill(FetchedSkills);
	InputSkill(SkillData);
}

void UUfSkillComponent::OnHold(const FInputActionInstance& InputActionInstance)
{
	const EUfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == EUfSkillKey::None)
		return;

	if(CurrentAction->GetSkillTable()->InputType != EUfInputType::Charge) // 차지 스킬일때는 딱히 아무것도 안해도 됨
	{
		// 키를 눌렀을때 다음에 뭐쓸지 여기서 결정하자. 결정을 바꾸면 로직이 복잡해진다.
		const TArray<FName> FetchedSkills = FetchSkillsByInput(SkillKey, ETriggerEvent::Ongoing);
		const FUfSkillData* SkillData = GetDesiredSkill(FetchedSkills);
		InputSkill(SkillData);
	}
}

void UUfSkillComponent::OnRelease(const FInputActionInstance& InputActionInstance)
{
	const EUfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == EUfSkillKey::None)
		return;

	if(CurrentAction)
	{
		CurrentAction->OnButtonReleased();
	}
}

bool UUfSkillComponent::CanCancelSkill(const FUfSkillData* SkillData) const
{
	// 특정 조건일때만 Cancel 되어야한다.
	return true;
}

bool UUfSkillComponent::CanPlaySkill(const FUfSkillData* SkillData) const
{
	if(SkillData == nullptr)
		return false;

	if(OwnerChar && OwnerChar->GetMovementComponent()->IsFalling())
		return false;

	// 게이지 체크는 여기서?

	if(CurrentAction == nullptr)
		return true;

	return true;
}

void UUfSkillComponent::InputSkill(const FUfSkillData* SkillData)
{
	switch (SkillState)
	{
	case EUfSkillState::None:
	case EUfSkillState::End:
	case EUfSkillState::Over:
		PlaySkill(SkillData);
		break;
	case EUfSkillState::PreInput:
		ReserveSkill(SkillData);
		break;
	case EUfSkillState::CanCancel:
		if(CanCancelSkill(SkillData))
		{
			PlaySkill(SkillData);
		}
		break;
	}
}

void UUfSkillComponent::PlaySkill(const FUfSkillData* SkillData)
{
	if(SkillData == nullptr)
		return;
	
	if(CanPlaySkill(SkillData))
	{
		PlayAction(SkillData->Montage, SkillData);
	}
}

void UUfSkillComponent::PlayAction(UAnimMontage* InMontage, const FUfSkillData* Skill)
{
	// 평타, 스킬, 점프, 맞기등 몽타주 관련된건 여기서 해야한다.
	ClearAction();
	
	SetSkillState(EUfSkillState::PreDelay);
	CurrentAction = NewObject<UUfActionBase>();
	CurrentAction->InitAction(OwnerChar, this, InMontage, Skill);
	CurrentAction->OnBegin();
}

void UUfSkillComponent::TickAction()
{
	if(CurrentAction)
	{
		CurrentAction->OnTick();
		if(CurrentAction->IsEnd())
		{
			//UF_LOG(TEXT("IsEnd"));
			ClearAction();
		}
	}
}

void UUfSkillComponent::ClearAction()
{
	if(CurrentAction)
	{
		SetSkillState(EUfSkillState::None);
		CurrentAction->OnEnd();
		CurrentAction = nullptr;
	}
}

void UUfSkillComponent::OnMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	// UF_LOG(TEXT("OnMontageEnd1"));
	// if(CurrentAction)
	// {
	// 	// 여기서 CurrentAction 을 지우면 CurrentAction 이 사라지게 된다. 체인인지 아닌지 알수 없게 된다. 
	// 	CurrentAction->OnMontageEnd();
	// 	UF_LOG(TEXT("OnMontageEnd"));
	// }
}

void UUfSkillComponent::ReserveSkill(const FUfSkillData* SkillData)
{
	if(ReservedRowName == NAME_None && SkillData)
		ReservedRowName = SkillData->RowName;
}

void UUfSkillComponent::TickInput()
{
	// FUfInput Input;
	// while(InputQueue.Dequeue(Input))
	// {
	// 	ProcessInputKey(Input);
	// }
}


