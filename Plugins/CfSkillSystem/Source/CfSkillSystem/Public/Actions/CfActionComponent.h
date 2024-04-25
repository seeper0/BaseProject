// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "CfSkill.h"
#include "GameFramework/Character.h"
#include "CfActionComponent.generated.h"


/**
 * 액션을 실행하는 컴퍼넌트, 스킬이나 히트리액션을 관리한다.
 */
UCLASS( ClassGroup=(CombatFoundation), meta=(BlueprintSpawnableComponent) )
class CFSKILLSYSTEM_API UCfActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// uniform initialization 전용 임시 struct, PlayAction argument로 사용된다.
	struct FActionInfo
	{
		FActionInfo() : SkillData(nullptr) , IsValid(false) {}
		FActionInfo(bool InIsValid) : SkillData(nullptr), IsValid(InIsValid) {}
		FActionInfo(const FCfSkillData* InSkillData) : SkillData(InSkillData), IsValid(true) {}
		FActionInfo(const FCfDamageEvent& InDamageEvent) : SkillData(nullptr), DamageEvent(InDamageEvent), IsValid(true) {}

		FActionInfo(const FActionInfo& Info) { operator=(Info); }
		const FActionInfo& operator=(const FActionInfo& Info)
		{
			SkillData = Info.SkillData;
			FCfDamageEvent* Event = const_cast<FCfDamageEvent*>(&DamageEvent);
			*Event = Info.DamageEvent;
			IsValid = Info.IsValid;
			return *this;
		}

		const FCfSkillData* SkillData;
		const FCfDamageEvent DamageEvent;
		bool IsValid = false;
	};

public:
	inline static FName ComponentName = TEXT("SkillComponent");
	// Sets default values for this component's properties
	UCfActionComponent();
	template<class T>
	static T* GetComponentOwner(const UActorComponent* Component)
	{
		if(Component)
			return Cast<T>(Component->GetOwner());
		return nullptr;
	}
	template<class T>
	static T* GetComponentByClass(const AActor* Actor)
	{
		if(Actor)
		{
			return Actor->GetComponentByClass<T>();
		}
		return nullptr;
	}
	template<class T>
	static T* GetComponentByClass(const UActorComponent* Component)
	{
		if(Component && Component->GetOwner())
		{
			return GetComponentByClass<T>(Component->GetOwner());
		}
		return nullptr;
	}
	static UCfActionComponent* GetActionComponent(const AActor* Actor);
	static UCfActionComponent* GetActionComponent(const UActorComponent* Component);

protected:
	// Called when the game starts
	virtual void OnRegister() override;
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetSkillState(ECfSkillState InSkillState);
	ECfSkillState GetSkillState() const { return SkillState; }
	ACharacter* GetOwnerChar() const { return OwnerChar; }
	AController* GetController() const { return OwnerChar ? OwnerChar->Controller : nullptr; }
	class UCfActionBase* GetCurrentAction() const { return CurrentAction; }

	UAnimMontage* GetKnockBackMontage() const;
	UAnimMontage* GetDownMontage() const;
	UAnimMontage* GetAirborneMontage() const;	
	UAnimMontage* GetWakeupMontage() const;	

	const FCfSkillData* GetDesiredSkill(const TArray<FName>& RowNames, const FVector& InputDirection) const;
	void InputSkill(const FCfSkillData* InSkillData, const FVector& InputDirection);
	void ReleaseSkill(const ECfSkillKey InSkillKey);
	void PlayAction(const FActionInfo& ActionInfo, const FVector& InputDirection = FVector());
	void ReserveAction(const FActionInfo& ActionInfo);
	void StopSkill();
	void SetStun(const float InRecoveryTime, const float InStunPlayRate);
	bool IsSuperArmorActive() const;
	bool IsReservedNext() const;
	bool IsEndSkill() const;
	void SetChargeLevel(int32 InChargeLevel) { ChargeLevel = InChargeLevel; }
	int32 GetChargeLevel() const { return ChargeLevel; }
	void EnableJustTiming(bool bEnable);

private:
	void SetWeaponType(const ECfWeaponType NewWeaponType);
public:
	void SwitchWeapon(const ECfWeaponType NewWeaponType); 
	ECfWeaponType GetWeaponType() const { return WeaponType; }
	const FName& GetCharacterName() const { return CharacterName; }

private:
	bool CanCancelSkill(const FCfSkillData* InSkillData) const;
	bool CanPlaySkill(const FCfSkillData* InSkillData) const;
	void PlaySkill(const FCfSkillData* InSkillData, const FVector& InputDirection);
	void TickAction(float DeltaTime);
	void ClearAction();		
	void ReserveSkill(const FCfSkillData* InSkillData, const FVector& InputDirection);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	FName CharacterName;

	UPROPERTY(Transient)
	ECfWeaponType WeaponType = ECfWeaponType::Unarmed;

	const struct FCfCharacterData* CharacterData = nullptr;

	UPROPERTY(Transient)
	TMap<const UInputAction*, ECfSkillKey> SkillSlotCache;
	
	UPROPERTY(Transient)
	ECfSkillState SkillState = ECfSkillState::None;

	UPROPERTY(Transient)
	ACharacter* OwnerChar = nullptr;

	UPROPERTY(Transient)
	UAnimInstance* AnimInstance = nullptr;

	UPROPERTY(Transient)
	class UCfSkillInputComponent* InputComponent = nullptr;

	UPROPERTY(Transient)
	class UCfActionBase* CurrentAction = nullptr;

	UPROPERTY(Transient)
	FName ReservedRowName;
	FVector ReservedInputDirection;
	FActionInfo ReverseActionInfo;
	int32 ChargeLevel = -1;
	bool bJustTiming = false;

#pragma region HitList
public:
	bool HasActorInHitList(const ACharacter* InVictim) const;
	void PushHitActorList(ACharacter* InVictim);
	void ClearHitActorList();

private:
	UPROPERTY()
	TSet<ACharacter*> HitActorList;

#pragma endregion
};
