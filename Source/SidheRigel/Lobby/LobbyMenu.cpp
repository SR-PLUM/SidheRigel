// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

#include "SidheRigel/SidheRigelGameInstance.h"
#include "SidheRigel/Lobby/LobbyGameMode.h"

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

	if (!Lobby_StartButton) return false;
	Lobby_StartButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnLobbyStartButton);

	SidheRigelInstance = Cast<USidheRigelGameInstance>(GetGameInstance());
	if (SidheRigelInstance)
	{
		SidheRigelInstance->SetInputUI(TakeWidget());
	}

	LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode)
	{
		LobbyGameMode->AddUIList(this);

		LobbyGameMode->RefreshPlayerText();
	}

	return true;
}

void ULobbyMenu::OpenLobbyMenu()
{
	if (LobbySwitcher == nullptr) return;
	LobbySwitcher->SetActiveWidget(LobbyMenu);
}

void ULobbyMenu::OpenCharacterSelectMenu()
{
	if (LobbySwitcher == nullptr) return;
	
	LobbySwitcher->SetActiveWidget(CharacterSelectMenu);
}

void ULobbyMenu::OnLobbyStartButton()
{
	LobbyGameMode->OpenCharacterSelectMenu(GetOwningPlayer());
}

void ULobbyMenu::RefreshPlayerList(TArray<class APlayerController*> playerList)
{
	int32 cnt = 0;
	for (auto& player : playerList)
	{
		if (cnt == 0)
		{
			Player_1->SetText(FText::FromString(player->PlayerState->UniqueId->ToString()));
		}
		else if (cnt == 1)
		{
			Player_2->SetText(FText::FromString(player->PlayerState->UniqueId->ToString()));
		}
		cnt++;
	}
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
