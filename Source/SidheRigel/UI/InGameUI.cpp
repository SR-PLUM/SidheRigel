// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "CharacterStatus.h"
#include "CombatSummary.h"
#include "SidheRigel/SidheRigelGameInstance.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "Components/Overlay.h"


void UInGameUI::InitCharacterData(USidheRigelGameInstance* GameInstance)
{

	GameInstanceRef = GameInstance;

	APlayerController* PlayerController = GameInstanceRef->GetFirstLocalPlayerController();

	CharacterRef = Cast<ASidheRigelCharacter>(PlayerController->GetCharacter());

	if (CharacterStatus != nullptr)
	{
		CharacterStatus->InitIconImage(GameInstanceRef->CharacterNum);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterStatus is null"));
	}
}

