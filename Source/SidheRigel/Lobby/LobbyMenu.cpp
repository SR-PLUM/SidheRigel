// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMenu.h"

#include "Components/Button.h"

#include "SidheRigel/SidheRigelGameInstance.h"

#include "SidheRigel/Enum/E_Character.h"

bool ULobbyMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ACMButton) return false;
	ACMButton->OnClicked.AddDynamic(this, &ULobbyMenu::SetCharacterACM);

	if (!BlackWizardButton) return false;
	BlackWizardButton->OnClicked.AddDynamic(this, &ULobbyMenu::SetCharacterBlackWizard);

	if (!ColdButton) return false;
	ColdButton->OnClicked.AddDynamic(this, &ULobbyMenu::SetCharacterCold);

	if (!FairyWingButton) return false;
	FairyWingButton->OnClicked.AddDynamic(this, &ULobbyMenu::SetCharacterFairyWing);

	if (!KerunButton) return false;
	KerunButton->OnClicked.AddDynamic(this, &ULobbyMenu::SetCharacterKerun);

	if (!StartGameButton) return false;
	StartGameButton->OnClicked.AddDynamic(this, &ULobbyMenu::StartGame);

	SidheRigelInstance = Cast<USidheRigelGameInstance>(GetGameInstance());
	if (SidheRigelInstance)
	{
		SidheRigelInstance->SetInputUI(TakeWidget());
	}

	return true;
}

void ULobbyMenu::SetCharacterACM()
{
	SidheRigelInstance->CharacterNum = ACM;
}

void ULobbyMenu::SetCharacterBlackWizard()
{
	SidheRigelInstance->CharacterNum = BlackWizard;
}

void ULobbyMenu::SetCharacterCold()
{
	SidheRigelInstance->CharacterNum = Cold;
}

void ULobbyMenu::SetCharacterFairyWing()
{
	SidheRigelInstance->CharacterNum = FairyWing;
}

void ULobbyMenu::SetCharacterKerun()
{
	SidheRigelInstance->CharacterNum = Kerun;
}

void ULobbyMenu::StartGame()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	World->ServerTravel("/Game/Maps/TrainingRoom?listen");
	//World->ServerTravel("/Game/Maps/TwistedDesert?listen");
	//World->ServerTravel("/Game/TopDown/Maps/TopDownMap?listen");
}
