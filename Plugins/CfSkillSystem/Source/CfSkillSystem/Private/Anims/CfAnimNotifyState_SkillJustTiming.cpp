// Fill out your copyright notice in the Description page of Project Settings.


#include "Anims/CfAnimNotifyState_SkillJustTiming.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/UObjectToken.h"
#include "Particles/ParticleSystem.h"
#include "Actions/CfActionComponent.h"
#include "CfLogger.h"
#include "Particles/ParticleSystemComponent.h"

UCfAnimNotifyState_SkillJustTiming::UCfAnimNotifyState_SkillJustTiming(const FObjectInitializer& ObjectInitializer)
{
	Attached = true;
	Scale = FVector(1.f);

#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(132, 150, 255, 255);
#endif // WITH_EDITORONLY_DATA
}

void UCfAnimNotifyState_SkillJustTiming::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if(ACharacter* Owner = UCfActionComponent::GetComponentOwner<ACharacter>(MeshComp))
	{
		ActionComponent = UCfActionComponent::GetActionComponent(MeshComp);
		if(ActionComponent)
			ActionComponent->EnableJustTiming(true);
	}
	ParticleSystem = SpawnParticleSystem(MeshComp, Animation);
}

void UCfAnimNotifyState_SkillJustTiming::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if(ActionComponent)
		ActionComponent->EnableJustTiming(false);
	// if(ParticleSystem)
	// 	ParticleSystem->DestroyComponent();
}

void UCfAnimNotifyState_SkillJustTiming::PostLoad()
{
	Super::PostLoad();

	RotationOffsetQuat = FQuat(RotationOffset);
}

#if WITH_EDITOR
void UCfAnimNotifyState_SkillJustTiming::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.MemberProperty && PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UCfAnimNotifyState_SkillJustTiming, RotationOffset))
	{
		RotationOffsetQuat = FQuat(RotationOffset);
	}
}

void UCfAnimNotifyState_SkillJustTiming::ValidateAssociatedAssets()
{
	static const FName NAME_AssetCheck("AssetCheck");

	if ((PSTemplate != nullptr) && (PSTemplate->IsLooping()))
	{
		UObject* ContainingAsset = GetContainingAsset();
			
		FMessageLog AssetCheckLog(NAME_AssetCheck);

		const FText MessageLooping = FText::Format(
			NSLOCTEXT("AnimNotify", "ParticleSystem_ShouldNotLoop", "Particle system {0} used in anim notify for asset {1} is set to looping, but the slot is a one-shot (it won't be played to avoid leaking a component per notify)."),
			FText::AsCultureInvariant(PSTemplate->GetPathName()),
			FText::AsCultureInvariant(ContainingAsset->GetPathName()));
		AssetCheckLog.Warning()
			->AddToken(FUObjectToken::Create(ContainingAsset))
			->AddToken(FTextToken::Create(MessageLooping));

		if (GIsEditor)
		{
			AssetCheckLog.Notify(MessageLooping, EMessageSeverity::Warning, /*bForce=*/ true);
		}
	}
}
#endif

UParticleSystemComponent* UCfAnimNotifyState_SkillJustTiming::SpawnParticleSystem(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	UParticleSystemComponent* ReturnComp = nullptr;

	if (PSTemplate)
	{
		if (PSTemplate->IsLooping())
		{
			CF_LOG_WARNING(TEXT("Particle Notify: Anim '%s' tried to spawn infinitely looping particle system '%s'. Spawning suppressed."), *GetNameSafe(Animation), *GetNameSafe(PSTemplate));
			return ReturnComp;
		}

		if (Attached)
		{
			ReturnComp = UGameplayStatics::SpawnEmitterAttached(PSTemplate, MeshComp, SocketName, LocationOffset, RotationOffset, Scale);
		}
		else
		{
			const FTransform MeshTransform = MeshComp->GetSocketTransform(SocketName);
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(MeshTransform.TransformPosition(LocationOffset));
			SpawnTransform.SetRotation(MeshTransform.GetRotation() * RotationOffsetQuat);
			SpawnTransform.SetScale3D(Scale);
			ReturnComp = UGameplayStatics::SpawnEmitterAtLocation(MeshComp->GetWorld(), PSTemplate, SpawnTransform);
		}
	}
	// else
	// {
	// 	CF_LOG_WARNING( TEXT("Particle Notify: Particle system is null for particle notify '%s' in anim: '%s'"), *GetNotifyName(), *GetPathNameSafe(Animation));
	// }

	return ReturnComp;
}
