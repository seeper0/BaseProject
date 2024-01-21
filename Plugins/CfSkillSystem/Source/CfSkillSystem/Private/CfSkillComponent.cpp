// Fill out your copyright notice in the Description page of Project Settings.


#include "CfSkillComponent.h"
#include "InputAction.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CfActionBase.h"
#include "CfLogger.h"
#include "CfSkillData.h"
#include "CfUtil.h"

// Sets default values for this component's properties
UCfSkillComponent::UCfSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

UCfSkillComponent* UCfSkillComponent::GetSkillComponent(const AActor* Actor)
{
	if(Actor)
	{
		return Actor->GetComponentByClass<UCfSkillComponent>();
	}
	return nullptr;
}

UCfSkillComponent* UCfSkillComponent::GetSkillComponent(const UActorComponent* Component)
{
	if(Component && Component->GetOwner())
	{
		return GetSkillComponent(Component->GetOwner());
	}
	return nullptr;
}

// Called when the game starts
void UCfSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerChar = Cast<ACharacter>(GetOwner());
	// if(OwnerChar && OwnerChar->GetMesh())
	// {
	// 	AnimInstance = OwnerChar->GetMesh()->GetAnimInstance();
	// 	if(AnimInstance)
	// 	{
	// 		AnimInstance->OnMontageEnded.AddDynamic(this, &UCfSkillComponent::OnMontageEnd);
	// 	}
	// }
}


// Called every frame
void UCfSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//InputQueue.Empty();

	TickInput();
	TickAction();
}

void UCfSkillComponent::SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent)
{
	SkillSlotCache.Empty();
	for (const auto& SkillSlot : SkillSlotMapping)
	{
		switch (SkillSlot.Key)
		{
		case ECfSkillKey::Move: // Moving
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Triggered, this, &UCfSkillComponent::Move);
			break;
		case ECfSkillKey::Look: // Looking
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Triggered, this, &UCfSkillComponent::Look);
			break;
		default:
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Started, this, &UCfSkillComponent::OnPress);
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Ongoing, this, &UCfSkillComponent::OnHold);
			EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Completed, this, &UCfSkillComponent::OnRelease);
			SkillSlotCache.Add(SkillSlot.Value, SkillSlot.Key);
			break;
		}
	}
}

void UCfSkillComponent::SetSkillState(ECfSkillState InSkillState)
{
	SkillState = InSkillState;
	// if(Action)
	// 	Action->SetSkillState(InSkillState);

	// 선입력으로 예약된 스킬을 발동한다.
	if(SkillTable && SkillState == ECfSkillState::CanCancel)
	{
		if(ReservedRowName != NAME_None)
		{
			const FCfSkillData* SkillData = SkillTable->FindRow<FCfSkillData>(ReservedRowName, CF_FUNCTION);
			if(CanCancelSkill(SkillData))
			{
				PlaySkill(SkillData);
			}
			ReservedRowName = NAME_None;
		}
	}
}

ECfSkillKey UCfSkillComponent::GetSkillSlot(const FInputActionInstance& InputActionInstance) const
{
	const ECfSkillKey* SkillKey = SkillSlotCache.Find(InputActionInstance.GetSourceAction());
	if(SkillKey == nullptr)
		return ECfSkillKey::None;

	return *SkillKey;
}

TArray<FName> UCfSkillComponent::FetchSkillsByInput(const ECfSkillKey SkillKey, const ETriggerEvent KeyEvent) const
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

const FCfSkillData* UCfSkillComponent::GetDesiredSkill(const TArray<FName>& RowNames) const
{
	// 체인 조건이 있다면 먼저 찾는다. (일단 현재 실행되는 스킬이 있어야함)
	if(CurrentAction)
	{
		const FName CurrentName = CurrentAction->GetActionName();
		for (const FName RowName : RowNames)
		{
			const FCfSkillData* RowData = SkillTable->FindRow<FCfSkillData>(RowName, CF_FUNCTION);
			if(RowData->CanChain(CurrentName))
			{
				return  RowData;
			}
		}
	}
	// 체인조건이 없다면 평타 조건 찾는다.
	for (const FName RowName : RowNames)
	{
		const FCfSkillData* RowData = SkillTable->FindRow<FCfSkillData>(RowName, CF_FUNCTION);
		if(RowData->NotChain())
		{
			return  RowData;
		}
	}

	// 스킬이 많아지면 Priority 로... Chain 이 먼저, Cancel기 다음, 마지막 평타...

	return nullptr;
}

void UCfSkillComponent::Move(const FInputActionValue& Value)
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

void UCfSkillComponent::Look(const FInputActionValue& Value)
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

void UCfSkillComponent::OnPress(const FInputActionInstance& InputActionInstance)
{
	const ECfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == ECfSkillKey::None)
		return;

	// 키를 눌렀을때 다음에 뭐쓸지 여기서 결정하자. 결정을 바꾸면 로직이 복잡해진다.
	const TArray<FName> FetchedSkills = FetchSkillsByInput(SkillKey, ETriggerEvent::Started);
	const FCfSkillData* SkillData = GetDesiredSkill(FetchedSkills);
	InputSkill(SkillData);
}

void UCfSkillComponent::OnHold(const FInputActionInstance& InputActionInstance)
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

void UCfSkillComponent::OnRelease(const FInputActionInstance& InputActionInstance)
{
	const ECfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == ECfSkillKey::None)
		return;

	if(CurrentAction)
	{
		CurrentAction->OnButtonReleased(SkillKey);
	}
}

bool UCfSkillComponent::CanCancelSkill(const FCfSkillData* InSkillData) const
{
	// 특정 조건일때만 Cancel 되어야한다.
	return true;
}

bool UCfSkillComponent::CanPlaySkill(const FCfSkillData* InSkillData) const
{
	if(InSkillData == nullptr)
		return false;

	if(OwnerChar && OwnerChar->GetMovementComponent()->IsFalling())
		return false;

	// 게이지 체크는 여기서?

	if(CurrentAction == nullptr)
		return true;

	return true;
}

void UCfSkillComponent::InputSkill(const FCfSkillData* InSkillData)
{
	switch (SkillState)
	{
	case ECfSkillState::None:
	case ECfSkillState::End:
	case ECfSkillState::Over:
		PlaySkill(InSkillData);
		break;
	case ECfSkillState::PreInput:
		ReserveSkill(InSkillData);
		break;
	case ECfSkillState::CanCancel:
		if(CanCancelSkill(InSkillData))
		{
			PlaySkill(InSkillData);
		}
		break;
	}
}

void UCfSkillComponent::PlaySkill(const FCfSkillData* InSkillData)
{
	if(InSkillData == nullptr)
		return;

	if(CanPlaySkill(InSkillData))
	{
		PlayAction(InSkillData->Montage, InSkillData);
	}
}

void UCfSkillComponent::PlayAction(UAnimMontage* InMontage, const FCfSkillData* InSkillData)
{
	// 평타, 스킬, 점프, 맞기등 몽타주 관련된건 여기서 해야한다.
	ClearAction();

	SetSkillState(ECfSkillState::PreDelay);
	CurrentAction = UCfActionBase::NewSkill(OwnerChar, this, InSkillData);
	CurrentAction->OnBegin();
}

void UCfSkillComponent::TickAction()
{
	if(CurrentAction)
	{
		CurrentAction->OnTick();
		if(CurrentAction->IsEnd())
		{
			//CF_LOG(TEXT("IsEnd"));
			ClearAction();
		}
	}
}

void UCfSkillComponent::ClearAction()
{
	if(CurrentAction)
	{
		SetSkillState(ECfSkillState::None);
		CurrentAction->OnEnd();
		CurrentAction = nullptr;
	}
}

void UCfSkillComponent::OnMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	// CF_LOG(TEXT("OnMontageEnd1"));
	// if(CurrentAction)
	// {
	// 	// 여기서 CurrentAction 을 지우면 CurrentAction 이 사라지게 된다. 체인인지 아닌지 알수 없게 된다. 
	// 	CurrentAction->OnMontageEnd();
	// 	CF_LOG(TEXT("OnMontageEnd"));
	// }
}

void UCfSkillComponent::ReserveSkill(const FCfSkillData* InSkillData)
{
	if(ReservedRowName == NAME_None && InSkillData)
		ReservedRowName = InSkillData->RowName;
}

void UCfSkillComponent::TickInput()
{
}

#pragma region HitList
bool UCfSkillComponent::HasActorInHitList(const ACharacter* InVictim) const
{
	return HitActorList.Contains(InVictim);
}

void UCfSkillComponent::PushHitActorList(ACharacter* InVictim)
{
	HitActorList.Add(InVictim);
}

void UCfSkillComponent::ClearHitActorList()
{
	HitActorList.Empty();
}
#pragma endregion

