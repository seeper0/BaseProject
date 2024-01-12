// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define UF_FUNCTION ANSI_TO_TCHAR( __FUNCTION__ )

#define UF_LOG( Format, ... )			UE_LOG( FnLog, Log, TEXT("[%s] %s"), UF_FUNCTION, *FString::Printf(Format, ##__VA_ARGS__))
#define UF_LOG_WARNING( Format, ... )	UE_LOG( FnLog, Warning, TEXT("[%s] %s"), UF_FUNCTION, *FString::Printf(Format, ##__VA_ARGS__))
#define UF_LOG_ERROR( Format, ... )		UE_LOG( FnLog, Error, TEXT("[%s] %s"), UF_FUNCTION, *FString::Printf(Format, ##__VA_ARGS__))

UFCOMMON_API DECLARE_LOG_CATEGORY_EXTERN( FnLog, Log, All );
