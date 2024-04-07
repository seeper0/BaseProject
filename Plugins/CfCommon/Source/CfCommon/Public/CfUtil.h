// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

struct FCfUtil
{
	template<typename EnumType>
	static FString GetEnumString( const EnumType EnumeratorValue )
	{
		return UEnum::GetDisplayValueAsText(EnumeratorValue).ToString();
	}

	static FVector GetCapsuleBottomLocation(const AActor* InActor)
	{
		const ACharacter* Character = Cast<ACharacter>(InActor);
		if(Character)
		{
			return Character->GetActorLocation() - FVector(0, 0, Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		}
		
		if(InActor)
		{
			return InActor->GetActorLocation();
		}

		return FVector::ZeroVector;
	}
};
