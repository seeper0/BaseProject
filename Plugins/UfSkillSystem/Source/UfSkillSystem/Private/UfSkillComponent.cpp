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

	if(SkillState == EUfSkillState::CanCancel)
	{
		FUfInput Input;
		while(InputQueue.Dequeue(Input))
		{
			ProcessPreInputKey(Input);
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

const FUfSkillData* UUfSkillComponent::FindSkill(const EUfSkillKey SkillKey) const
{
	if(SkillTable == nullptr)
		return nullptr;

	TArray<FName> RowNames = SkillTable->GetRowNames();
	for (const FName RowName : RowNames)
	{
		const FUfSkillData* RowData = SkillTable->FindRow<FUfSkillData>(RowName, UF_FUNCTION);
		if(RowData == nullptr)
			continue;
		
		UF_LOG(TEXT("Table %s : %s"), *FUfUtil::GetEnumString(RowData->Key), *FUfUtil::GetEnumString(SkillKey));
		if(RowData->Key == SkillKey)
		{
			return RowData;
		}
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

	switch (SkillState)
	{
	case EUfSkillState::None:
	case EUfSkillState::End:
		PlaySkill(SkillKey, ETriggerEvent::Started);
		break;
	case EUfSkillState::PreInput:
		QueueSkill(SkillKey, ETriggerEvent::Started);
		break;
	case EUfSkillState::CanCancel:
		if(CanCancelSkill(SkillKey, ETriggerEvent::Started))
		{
			PlaySkill(SkillKey, ETriggerEvent::Started);
		}
		break;
	}
	
}

void UUfSkillComponent::OnTrigger(const FInputActionInstance& InputActionInstance)
{
	const EUfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == EUfSkillKey::None)
		return;

	//InputQueue.Enqueue({SkillKey, ETriggerEvent::Triggered});
}

void UUfSkillComponent::OnRelease(const FInputActionInstance& InputActionInstance)
{
	const EUfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == EUfSkillKey::None)
		return;

	//InputQueue.Enqueue({SkillKey, ETriggerEvent::Completed});
}

bool UUfSkillComponent::CanCancelSkill(EUfSkillKey SkillKey, ETriggerEvent Started) const
{
	// 특정 조건일때만 Cancel 되어야한다.
	return true;
}

bool UUfSkillComponent::CanPlaySkill(const FUfSkillData* Skill) const
{
	if(OwnerChar && OwnerChar->GetMovementComponent()->IsFalling())
		return false;

	// 게이지 체크는 여기서?

	if(CurrentAction == nullptr)
		return true;

	return true;
}

void UUfSkillComponent::PlaySkill(EUfSkillKey SkillKey, ETriggerEvent Started)
{
	// 현재상태와 키를 비교해서 스킬을 찾아낸다.
	// Chain Skill 일 경우 Chain Skill을 가져온다.

	// 1. 그냥 이면 스킬 쓴다.
	// 2. 체인 조건이면 체인을 쓴다.
	// 3. 가장 합리적인 찾는건 무엇일까?
	// 4. 쿨타임이나 자원을 소모하면 어떻게 될까?

	//조건들을 정리하자
	// 땅에 있다면
	// 사용하는 스킬이 없다면
	// 쿨타임이 가능하다면
	// 자원이 가능하다면
	// 특정 타이밍 입력 조건이 맞춰줬다면
	// 선입력 or 즉시 발동?

	UF_LOG(TEXT("A [%p]"), CurrentAction);
	const FUfSkillData* SkillData = nullptr;
	// 현재 스킬이 없다면...
	if(CurrentAction)
	{
		UF_LOG(TEXT("CurrentAction : %s"), *CurrentAction->ToString());
		SkillData = FindChainSkill(SkillKey);
	}
	UF_LOG(TEXT("B"));

	if(SkillData == nullptr)
	{
	UF_LOG(TEXT("C"));
		SkillData = FindSkill(SkillKey);
	}

	if(SkillData)
	{
		if(CanPlaySkill(SkillData))
		{
			PlayAction(SkillData->Montage, SkillData);
		}
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

void UUfSkillComponent::QueueSkill(EUfSkillKey SkillKey, ETriggerEvent Started)
{
	// 일단 1개만 받자
	if(InputQueue.IsEmpty())
	{
		InputQueue.Enqueue({SkillKey, ETriggerEvent::Started});
	}
}

void UUfSkillComponent::TickInput()
{
	// FUfInput Input;
	// while(InputQueue.Dequeue(Input))
	// {
	// 	ProcessInputKey(Input);
	// }
}

void UUfSkillComponent::ProcessPreInputKey(const FUfInput& Input)
{
	if(CanCancelSkill(Input.SkillKey, Input.Event))
	{
		PlaySkill(Input.SkillKey, Input.Event);
		InputQueue.Empty();
	}
}


