// Fill out your copyright notice in the Description page of Project Settings.


#include "UfSkillComponent.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "UfActionBase.h"
#include "UfLogger.h"
#include "UfSkillTable.h"
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
	if(OwnerChar && OwnerChar->GetMesh())
	{
		AnimInstance = OwnerChar->GetMesh()->GetAnimInstance();
		if(AnimInstance)
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &UUfSkillComponent::OnMontageEnd);
		}
	}
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
}

EUfSkillKey UUfSkillComponent::GetSkillSlot(const FInputActionInstance& InputActionInstance) const
{
	const EUfSkillKey* SkillKey = SkillSlotCache.Find(InputActionInstance.GetSourceAction());
	if(SkillKey == nullptr)
		return EUfSkillKey::None;

	return *SkillKey;
}

const FUfSkillTable* UUfSkillComponent::FindSkill(const EUfSkillKey SkillKey) const
{
	const FUfSkillTable* Skill = nullptr;
	if(SkillTable)
	{
		SkillTable->ForeachRow<FUfSkillTable>(TEXT("UUfSkillComponent::OnPress"), [this, SkillKey, &Skill](const FName& Key, const FUfSkillTable& Value) mutable
		{
			//UF_LOG(TEXT("Table %s : %s"), *FUfUtil::GetEnumString(Value.Key), *FUfUtil::GetEnumString(SkillKey));
			if(Value.Key == SkillKey)
				Skill = &Value;
		});
	}
	return Skill;
}

void UUfSkillComponent::OnPress(const FInputActionInstance& InputActionInstance)
{
	const EUfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == EUfSkillKey::None)
		return;

	//InputQueue.Enqueue(Input{Slot, ETriggerEvent::Started});

	// 스킬 클래스를 먼저 만들어야겠다.
	//PlayAction(SkillKey);
}

void UUfSkillComponent::OnTrigger(const FInputActionInstance& InputActionInstance)
{
	const EUfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == EUfSkillKey::None)
		return;

	//InputQueue.Enqueue(Input{Slot, ETriggerEvent::Triggered});
}

void UUfSkillComponent::OnRelease(const FInputActionInstance& InputActionInstance)
{
	const EUfSkillKey SkillKey = GetSkillSlot(InputActionInstance);
	if(SkillKey == EUfSkillKey::None)
		return;

	//InputQueue.Enqueue(Input{Slot, ETriggerEvent::Completed});
}

bool UUfSkillComponent::CanAction(const FUfSkillTable* Skill) const
{
	if(OwnerChar && OwnerChar->GetMovementComponent()->IsFalling())
		return false;

	// 게이지 체크는 여기서?

	if(Action == nullptr)
		return true;

	switch (SkillState)
	{
	case EUfSkillState::None:
		return false;
	case EUfSkillState::PreDelay:
		return false;
	case EUfSkillState::PreInput:
		return true;
	case EUfSkillState::Cancel:
		return true;
	case EUfSkillState::End:
		return true;
	}

	return true;
}

void UUfSkillComponent::PlayAction(const EUfSkillKey SkillKey)
{
	const FUfSkillTable* Skill = FindSkill(SkillKey);
	if(Skill)
	{
		if(CanAction(Skill))
		{
			ClearAction();
			Action = NewObject<UUfActionBase>();
			Action->InitAction(OwnerChar, this, Skill->Montage);
			Action->OnBegin();
		}
	}
}

void UUfSkillComponent::TickAction()
{
	if(Action)
	{
		Action->OnTick();
		if(Action->IsEnd())
		{
			ClearAction();
		}
	}
}

void UUfSkillComponent::ClearAction()
{
	if(Action)
	{
		Action->OnEnd();
		Action = nullptr;
	}
}

void UUfSkillComponent::OnMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if(Action)
	{
		Action->OnMontageEnd();
		ClearAction();
	}
}

void UUfSkillComponent::TickInput()
{
	// switch (SkillState)
	// {
	// case EUfSkillState::None:
	// case EUfSkillState::PreDelay:
	// 	return false;
	// case EUfSkillState::PreInput:
	// 	return true;
	// case EUfSkillState::Cancel:
	// 	return true;
	// case EUfSkillState::End:
	// 	return true;
	// }
}


