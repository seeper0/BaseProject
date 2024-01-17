// Fill out your copyright notice in the Description page of Project Settings.


#include "UfPlayerController.h"
#include "UfCheatManager.h"

AUfPlayerController::AUfPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//PlayerCameraManagerClass
	CheatClass = UUfCheatManager::StaticClass();
}
