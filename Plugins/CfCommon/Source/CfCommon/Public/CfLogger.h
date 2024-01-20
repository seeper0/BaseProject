// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define CF_FUNCTION ANSI_TO_TCHAR( __FUNCTION__ )

#define CF_LOG( Format, ... )			UE_LOG( CfLog, Log, TEXT("[%s] %s"), CF_FUNCTION, *FString::Printf(Format, ##__VA_ARGS__))
#define CF_LOG_WARNING( Format, ... )	UE_LOG( CfLog, Warning, TEXT("[%s] %s"), CF_FUNCTION, *FString::Printf(Format, ##__VA_ARGS__))
#define CF_LOG_ERROR( Format, ... )		UE_LOG( CfLog, Error, TEXT("[%s] %s"), CF_FUNCTION, *FString::Printf(Format, ##__VA_ARGS__))

CFCOMMON_API DECLARE_LOG_CATEGORY_EXTERN( CfLog, Log, All );
