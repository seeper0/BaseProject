// Fill out your copyright notice in the Description page of Project Settings.


#include "CfCheatManager.h"

void UCfCheatManager::TogglePlayerInfo()
{
	bShowPlayerInfo = !bShowPlayerInfo;
}

void UCfCheatManager::ToggleHitBox()
{
	bShowHitBox = !bShowHitBox;
}
