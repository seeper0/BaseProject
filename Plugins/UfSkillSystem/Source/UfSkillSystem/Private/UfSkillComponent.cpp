// Fill out your copyright notice in the Description page of Project Settings.


#include "UfSkillComponent.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "UfLogger.h"
#include "UfSkillTable.h"

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
}


// Called every frame
void UUfSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//InputQueue.Empty();
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
			UF_LOG(TEXT("Table %s : %s"), *UEnum::GetValueAsString<EUfSkillKey>(Value.Key), *UEnum::GetValueAsString<EUfSkillKey>(SkillKey));
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
	const FUfSkillTable* Skill = FindSkill(SkillKey);
	if(Skill)
	{
		OwnerChar->PlayAnimMontage(Skill->Montage);
	}
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


