// Fill out your copyright notice in the Description page of Project Settings.


#include "UfSkillComponent.h"

#include "EnhancedInputComponent.h"

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
}


// Called every frame
void UUfSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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

void UUfSkillComponent::SetSkillState(ESkillState InSkillState)
{
	SkillState = InSkillState;
}

ESkillSlot UUfSkillComponent::GetSkillSlot(const FInputActionInstance& InputActionInstance) const
{
	const ESkillSlot* Slot = SkillSlotCache.Find(InputActionInstance.GetSourceAction());
	if(Slot == nullptr)
		return ESkillSlot::None;

	return *Slot;
}

void UUfSkillComponent::OnPress(const FInputActionInstance& InputActionInstance)
{
	ESkillSlot Slot = GetSkillSlot(InputActionInstance);
	if(Slot == ESkillSlot::None)
		return;
}

void UUfSkillComponent::OnTrigger(const FInputActionInstance& InputActionInstance)
{
	ESkillSlot Slot = GetSkillSlot(InputActionInstance);
	if(Slot == ESkillSlot::None)
		return;
}

void UUfSkillComponent::OnRelease(const FInputActionInstance& InputActionInstance)
{
	ESkillSlot Slot = GetSkillSlot(InputActionInstance);
	if(Slot == ESkillSlot::None)
		return;
}


