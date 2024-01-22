// Fill out your copyright notice in the Description page of Project Settings.


#include "CfActionComponent.h"
#include "InputAction.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CfActionBase.h"
#include "CfLogger.h"
#include "CfSkillData.h"
#include "CfUtil.h"

// Sets default values for this component's properties
UCfActionComponent::UCfActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

UCfActionComponent* UCfActionComponent::GetActionComponent(const AActor* Actor)
{
	if(Actor)
	{
		return Actor->GetComponentByClass<UCfActionComponent>();
	}
	return nullptr;
}

UCfActionComponent* UCfActionComponent::GetActionComponent(const UActorComponent* Component)
{
	if(Component && Component->GetOwner())
	{
		return GetActionComponent(Component->GetOwner());
	}
	return nullptr;
}

// Called when the game starts
void UCfActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerChar = Cast<ACharacter>(GetOwner());
	// if(OwnerChar && OwnerChar->GetMesh())
	// {
	// 	AnimInstance = OwnerChar->GetMesh()->GetAnimInstance();
	// 	if(AnimInstance)
	// 	{
	// 		AnimInstance->OnMontageEnded.AddDynamic(this, &UCfActionComponent::OnMontageEnd);
	// 	}
	// }
}


// Called every frame
void UCfActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//InputQueue.Empty();

	TickInput();
	TickAction();
}

void UCfActionComponent::SetSkillState(ECfSkillState InSkillState)
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

const FCfSkillData* UCfActionComponent::GetDesiredSkill(const TArray<FName>& RowNames) const
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

bool UCfActionComponent::CanCancelSkill(const FCfSkillData* InSkillData) const
{
	// 특정 조건일때만 Cancel 되어야한다.
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

void UCfActionComponent::InputSkill(const FCfSkillData* InSkillData)
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

void UCfActionComponent::PlaySkill(const FCfSkillData* InSkillData)
{
	if(InSkillData == nullptr)
		return;

	if(CanPlaySkill(InSkillData))
	{
		PlayAction(InSkillData->Montage, InSkillData);
	}
}

void UCfActionComponent::PlayAction(UAnimMontage* InMontage, const FCfSkillData* InSkillData)
{
	// 평타, 스킬, 점프, 맞기등 몽타주 관련된건 여기서 해야한다.
	ClearAction();

	SetSkillState(ECfSkillState::PreDelay);
	CurrentAction = UCfActionBase::NewSkill(OwnerChar, this, InSkillData);
	CurrentAction->OnBegin();
}

void UCfActionComponent::TickAction()
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

void UCfActionComponent::ClearAction()
{
	if(CurrentAction)
	{
		SetSkillState(ECfSkillState::None);
		CurrentAction->OnEnd();
		CurrentAction = nullptr;
	}
}

void UCfActionComponent::OnMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	// CF_LOG(TEXT("OnMontageEnd1"));
	// if(CurrentAction)
	// {
	// 	// 여기서 CurrentAction 을 지우면 CurrentAction 이 사라지게 된다. 체인인지 아닌지 알수 없게 된다. 
	// 	CurrentAction->OnMontageEnd();
	// 	CF_LOG(TEXT("OnMontageEnd"));
	// }
}

void UCfActionComponent::ReserveSkill(const FCfSkillData* InSkillData)
{
	if(ReservedRowName == NAME_None && InSkillData)
		ReservedRowName = InSkillData->RowName;
}

void UCfActionComponent::TickInput()
{
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

