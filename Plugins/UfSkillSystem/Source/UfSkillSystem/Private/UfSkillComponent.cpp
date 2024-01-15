// Fill out your copyright notice in the Description page of Project Settings.


#include "UfSkillComponent.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "UfActionBase.h"
#include "UfLogger.h"
#include "UfSkillData.h"
#include "UfUtil.h"
#include "GameFramework/PawnMovementComponent.h"

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
		EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Started, this, &UUfSkillComponent::OnPress);
		EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Triggered, this, &UUfSkillComponent::OnTrigger);
		EnhancedInputComponent->BindAction(SkillSlot.Value, ETriggerEvent::Completed, this, &UUfSkillComponent::OnRelease);
		
		SkillSlotCache.Add(SkillSlot.Value, SkillSlot.Key);
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

const FUfSkillData* UUfSkillComponent::FindSkill(const EUfSkillKey SkillKey, const ETriggerEvent KeyEvent) const
{
	if(SkillTable == nullptr)
		return nullptr;

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

				//UF_LOG(TEXT("Table %s : %s (%s/%s)"), *FUfUtil::GetEnumString(RowData->Key), *FUfUtil::GetEnumString(SkillKey), *RowData->GetRequireSkill(0).ToString(), *CurrentAction->GetSkillTable()->RowName.ToString());
				// CurrentAction, SkillKey, KeyEvent 조합으로 찾는다.
				// 1. SkillKey가 맞는가?
				// 2. 이벤트가 맞는가?
				// 3. GetRequireSkill() 스킬이 없다면 볼것 없고...
				// 3. GetRequireSkill() 스킬이 있다면 CurrentAction 과 조건이 맞는지 봐야함.
				if(RowData->Key == SkillKey)
				{
					if(KeyEvent == ETriggerEvent::Started ||
						RowData->InputType == EUfInputType::AutoCombo && KeyEvent == ETriggerEvent::Triggered)
					{
						if((CurrentAction && RowData->CanChain(CurrentAction->GetSkillTable()->RowName)) ||
							RowData->NotChain())
						{
							return RowData;
						}
					}
				}
			}
		}
		break;
	}
	
	return nullptr;
}

const FUfSkillData* UUfSkillComponent::FindChainSkill(EUfSkillKey SkillKey) const
{
	if(SkillTable == nullptr)
		return nullptr;

	TArray<FName> RowNames = SkillTable->GetRowNames();
	for (const FName RowName : RowNames)
	{
		const FUfSkillData* RowData = SkillTable->FindRow<FUfSkillData>(RowName, UF_FUNCTION);
		if(RowData == nullptr)
			continue;

		UF_LOG(TEXT("Table %s : %s (%s/%s)"), *FUfUtil::GetEnumString(RowData->Key), *FUfUtil::GetEnumString(SkillKey), *RowData->GetRequireSkill(0).ToString(), *CurrentAction->GetSkillTable()->RowName.ToString());
		if(RowData->Key == SkillKey)
		{
			if(CurrentAction && RowData->GetRequireSkill(0) == CurrentAction->GetSkillTable()->RowName)
			{
				return RowData;
			}
		}
	}
	
	return nullptr;
}

void UUfSkillComponent::OnPress(const FInputActionInstance& InputActionInstance)
{
	const EUfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == EUfSkillKey::None)
		return;

	// 키를 눌렀을때 다음에 뭐쓸지 여기서 결정이 나야한다. 중간에 키만넘기다 바꾸면 스킬을 여러번 바꿔야하는 불상사가 나올수 있다.
	const FUfSkillData* SkillData = FindSkill(SkillKey, ETriggerEvent::Started);

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

void UUfSkillComponent::OnTrigger(const FInputActionInstance& InputActionInstance)
{
	const EUfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == EUfSkillKey::None)
		return;

	// 키를 눌렀을때 다음에 뭐쓸지 여기서 결정이 나야한다. 중간에 키만넘기다 바꾸면 스킬을 여러번 바꿔야하는 불상사가 나올수 있다.
	const FUfSkillData* SkillData = FindSkill(SkillKey, ETriggerEvent::Triggered);

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

void UUfSkillComponent::OnRelease(const FInputActionInstance& InputActionInstance)
{
	const EUfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == EUfSkillKey::None)
		return;
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
			UF_LOG(TEXT("IsEnd"));
			ClearAction();
		}
	}
}

void UUfSkillComponent::ClearAction()
{
	if(CurrentAction)
	{
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


