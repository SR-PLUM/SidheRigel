// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "CharacterStatus.h"
#include "CombatSummary.h"

void UInGameUI::InitCharacterData(E_Character Character)
{
	CharacterStatus->InitIconImage(Character);
}
