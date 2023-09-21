// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "CharacterStatus.h"
#include "CombatSummary.h"
#include "SidheRigel/SidheRigelGameInstance.h"


void UInGameUI::InitCharacterData(E_Character Character)
{

	if (CharacterStatus != nullptr)
	{
		CharacterStatus->InitIconImage(Character);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterStatus is null"));
	}
	

	
}
