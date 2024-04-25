// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CfActionComponent.h"

#include "CfAvatarComponent.h"
#include "CfCharacterData.h"
#include "InputAction.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Actions/CfActionBase.h"
#include "CfLogger.h"
#include "CfSkillAsset.h"
#include "CfSkillData.h"
#include "CfSkillInputComponent.h"
#include "CfUtil.h"

// Sets default values for this component's properties
UCfActionComponent::UCfActionComponent()
	: ReverseActionInfo(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

UCfActionComponent* UCfActionComponent::GetActionComponent(const AActor* Actor)
{
	return GetComponentByClass<UCfActionComponent>(Actor);
}

UCfActionComponent* UCfActionComponent::GetActionComponent(const UActorComponent* Component)
{
	return GetComponentByClass<UCfActionComponent>(Component);
}

void UCfActionComponent::OnRegister()
{
	Super::OnRegister();

	OwnerChar = Cast<ACharacter>(GetOwner());
	check(OwnerChar);

	CharacterData = UCfSkillAsset::GetCharacterData(CharacterName, CF_FUNCTION);

	if(CharacterData)
	{
		if(UCfAvatarComponent* AvatarComponent = OwnerChar->GetComponentByClass<UCfAvatarComponent>())
		{
			AvatarComponent->Initialize(CharacterData);
		}
		SetWeaponType(CharacterData->WeaponType1);
	}

	InputComponent = OwnerChar->GetComponentByClass<UCfSkillInputComponent>();
}

// Called when the game starts
void UCfActionComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UCfActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TickAction(DeltaTime);
}

void UCfActionComponent::SetSkillState(ECfSkillState InSkillState)
{
	SkillState = InSkillState;
	// if(Action)
	// 	Action->SetSkillState(InSkillState);

	// 선입력으로 예약된 스킬을 발동한다.
	const UDataTable* SkillTable = UCfSkillAsset::GetSkillTable();
	if(SkillTable && SkillState == ECfSkillState::CanCancel)
	{
		if(ReservedRowName != NAME_None)
		{
			const FCfSkillData* SkillData = SkillTable->FindRow<FCfSkillData>(ReservedRowName, CF_FUNCTION);
			if(CanCancelSkill(SkillData))
			{
				PlaySkill(SkillData, ReservedInputDirection);
			}
			ReservedRowName = NAME_None;
			ReservedInputDirection = FVector();
		}
	}
}

UAnimMontage* UCfActionComponent::GetKnockBackMontage() const
{
	return CharacterData ? CharacterData->KnockBackMontage : nullptr;
}

UAnimMontage* UCfActionComponent::GetDownMontage() const
{
	return CharacterData ? CharacterData->DownMontage : nullptr;
}

UAnimMontage* UCfActionComponent::GetAirborneMontage() const
{
	return CharacterData ? CharacterData->AirborneMontage : nullptr;
}

UAnimMontage* UCfActionComponent::GetWakeupMontage() const
{
	return CharacterData ? CharacterData->WakeupMontage : nullptr;
}

const FCfSkillData* UCfActionComponent::GetDesiredSkill(const TArray<FName>& RowNames, const FVector& InputDirection) const
{
	// 체인 조건이 있다면 먼저 찾는다. (일단 현재 실행되는 스킬이 있어야함)
	const UDataTable* SkillTable = UCfSkillAsset::GetSkillTable();
	ECfCardinalDirection Direction = InputComponent->GetCardinalDirection(InputDirection);
	if(CurrentAction && SkillTable)
	{
		const FName CurrentName = CurrentAction->GetActionName();
		for (const FName RowName : RowNames)
		{
			const FCfSkillData* RowData = SkillTable->FindRow<FCfSkillData>(RowName, CF_FUNCTION);
			if(!RowData->IsDirectionValid(Direction))
			{
				continue;
			}
			
			if(RowData->CanChain(CurrentName, bJustTiming))
			{
				return  RowData;
			}
		}
	}
	// 체인조건이 없다면 평타 조건 찾는다.
	for (const FName RowName : RowNames)
	{
		const FCfSkillData* RowData = SkillTable->FindRow<FCfSkillData>(RowName, CF_FUNCTION);
		if(!RowData->IsDirectionValid(Direction))
		{
			continue;
		}

		if(RowData->NotChain())
		{
			return  RowData;
		}
	}

	// 스킬이 많아지면 Priority 로... Chain 이 먼저, Cancel기 다음, 마지막 평타...

	return nullptr;
}

bool UCfActionComponent::CanCancelSkill(const FCfSkillData* InSkillData) const
{
	// 무조건 불가능한 구간
	switch (SkillState)
	{
		case ECfSkillState::NoInput:
		case ECfSkillState::PreInput:
			return false;
	}

	// InSkillData 와 SkillTable 비교해서 우선권이 높다면 Cancel 가능
	// 나머지 모두 가능
	return true;
}

bool UCfActionComponent::CanPlaySkill(const FCfSkillData* InSkillData) const
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

void UCfActionComponent::InputSkill(const FCfSkillData* InSkillData, const FVector& InputDirection)
{
	if(InSkillData == nullptr)
		return;

	switch (SkillState)
	{
	case ECfSkillState::None:
	case ECfSkillState::End:
	case ECfSkillState::Over:
		PlaySkill(InSkillData, InputDirection);
		break;
	case ECfSkillState::PreInput:
		ReserveSkill(InSkillData, InputDirection);
		break;
	case ECfSkillState::CanCancel:
		if(CanCancelSkill(InSkillData))
		{
			PlaySkill(InSkillData, InputDirection);
		}
		break;
	}
}

void UCfActionComponent::ReleaseSkill(const ECfSkillKey InSkillKey)
{
	if(CurrentAction)
	{
		if(CurrentAction->HasSkillKey(InSkillKey))
		{
			CurrentAction->ReleaseButton(InSkillKey);
		}
	}
}

void UCfActionComponent::PlayAction(const FActionInfo& ActionInfo, const FVector& InputDirection)
{
	// 평타, 스킬, 점프, 맞기등 몽타주 관련된건 여기서 해야한다.
	ClearAction();

	SetSkillState(ECfSkillState::NoInput);
	if(ActionInfo.SkillData)
	{
		CurrentAction = UCfActionBase::NewSkill(OwnerChar, this, ActionInfo.SkillData, InputDirection);
	}
	else if(ActionInfo.DamageEvent.SkillData)
	{
		CurrentAction = UCfActionBase::NewHitReaction(OwnerChar, this, ActionInfo.DamageEvent);
	}
	else
	{
		CurrentAction = UCfActionBase::NewRecover(OwnerChar, this);
	}

	CurrentAction->Begin();
}

void UCfActionComponent::ReserveAction(const FActionInfo& ActionInfo)
{
	ReverseActionInfo = FActionInfo(ActionInfo);
}

void UCfActionComponent::StopSkill()
{
	ClearAction();
}

void UCfActionComponent::SetStun(const float InRecoveryTime, const float InStunPlayRate)
{
	if(CurrentAction)
		CurrentAction->SetStun(InRecoveryTime, InStunPlayRate);
}

bool UCfActionComponent::IsSuperArmorActive() const
{
	if(CurrentAction)
		return CurrentAction->IsSuperArmorActive();
	return false;
}

bool UCfActionComponent::IsReservedNext() const
{
	return ReservedRowName != NAME_None || ReverseActionInfo.IsValid;	
}

bool UCfActionComponent::IsEndSkill() const
{
	switch (SkillState)
	{
	case ECfSkillState::None:
	case ECfSkillState::End:
	case ECfSkillState::Over:
		return true;
	}
	return false;
}

void UCfActionComponent::EnableJustTiming(bool bEnable)
{
	bJustTiming = bEnable;
}

void UCfActionComponent::SetWeaponType(const ECfWeaponType NewWeaponType)
{
	WeaponType = NewWeaponType;
}

void UCfActionComponent::SwitchWeapon(const ECfWeaponType NewWeaponType)
{
	SetWeaponType(NewWeaponType);
	if(UCfAvatarComponent* AvatarComponent = OwnerChar->GetComponentByClass<UCfAvatarComponent>())
	{
		AvatarComponent->SwitchWeapon(NewWeaponType);
	}
}

void UCfActionComponent::PlaySkill(const FCfSkillData* InSkillData, const FVector& InputDirection)
{
	if(InSkillData == nullptr)
		return;

	if(CanPlaySkill(InSkillData))
	{
		PlayAction({InSkillData}, InputDirection);
	}
}

void UCfActionComponent::TickAction(float DeltaTime)
{
	if(CurrentAction)
	{
		CurrentAction->Tick(DeltaTime);
		if(CurrentAction->IsEnd())
		{
			//CF_LOG(TEXT("IsEnd"));
			ClearAction();

			if(ReverseActionInfo.IsValid)
			{
				PlayAction(ReverseActionInfo);
				ReverseActionInfo = FActionInfo();
			}
		}
	}
}

void UCfActionComponent::ClearAction()
{
	if(CurrentAction)
	{
		SetSkillState(ECfSkillState::None);
		CurrentAction->End();
		CurrentAction = nullptr;
	}
}

void UCfActionComponent::ReserveSkill(const FCfSkillData* InSkillData, const FVector& InputDirection)
{
	if(ReservedRowName == NAME_None && InSkillData)
	{
		ReservedRowName = InSkillData->RowName;
		ReservedInputDirection = InputDirection;
	}
}

#pragma region HitList
bool UCfActionComponent::HasActorInHitList(const ACharacter* InVictim) const
{
	return HitActorList.Contains(InVictim);
}

void UCfActionComponent::PushHitActorList(ACharacter* InVictim)
{
	HitActorList.Add(InVictim);
}

void UCfActionComponent::ClearHitActorList()
{
	HitActorList.Empty();
}
#pragma endregion

