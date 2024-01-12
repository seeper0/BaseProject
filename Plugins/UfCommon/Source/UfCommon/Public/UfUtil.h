// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FUfUtil
{
	template<typename EnumType>
	static FString GetEnumString( const EnumType EnumeratorValue )
	{
		return UEnum::GetDisplayValueAsText(EnumeratorValue).ToString();
	}
};