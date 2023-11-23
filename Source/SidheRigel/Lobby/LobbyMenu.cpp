// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

#include "SidheRigel/SidheRigelGameInstance.h"
#include "SidheRigel/Lobby/LobbyGameMode.h"
#include "SidheRigel/Lobby/LobbyPlayerController.h"

#include "SidheRigel/Enum/E_Character.h"

bool ULobbyMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (ACMButton)
	{
		ACMButton->OnClicked.AddDynamic(this, &ULobbyMenu::SetCharacterACM);

		ACMButton->WidgetStyle.Normal.SetImageSize(FVector2D(200, 200));
		ACMButton->WidgetStyle.Hovered.SetImageSize(FVector2D(210, 210));
		ACMButton->WidgetStyle.Pressed.SetImageSize(FVector2D(190, 190));
	}
	

	if (BlackWizardButton)
	{
		BlackWizardButton->OnClicked.AddDynamic(this, &ULobbyMenu::SetCharacterBlackWizard);

		BlackWizardButton->WidgetStyle.Normal.SetImageSize(FVector2D(200, 200));
		BlackWizardButton->WidgetStyle.Hovered.SetImageSize(FVector2D(210, 210));
		BlackWizardButton->WidgetStyle.Pressed.SetImageSize(FVector2D(190, 190));
	}
	

	if (ColdButton)
	{
		ColdButton->OnClicked.AddDynamic(this, &ULobbyMenu::SetCharacterCold);

		ColdButton->WidgetStyle.Normal.SetImageSize(FVector2D(200, 200));
		ColdButton->WidgetStyle.Hovered.SetImageSize(FVector2D(210, 210));
		ColdButton->WidgetStyle.Pressed.SetImageSize(FVector2D(190, 190));
	}
	

	if (FairyWingButton)
	{
		FairyWingButton->OnClicked.AddDynamic(this, &ULobbyMenu::SetCharacterFairyWing);

		FairyWingButton->WidgetStyle.Normal.SetImageSize(FVector2D(200, 200));
		FairyWingButton->WidgetStyle.Hovered.SetImageSize(FVector2D(210, 210));
		FairyWingButton->WidgetStyle.Pressed.SetImageSize(FVector2D(190, 190));
	}
	

	if (KerunButton)
	{
		KerunButton->OnClicked.AddDynamic(this, &ULobbyMenu::SetCharacterKerun);

		KerunButton->WidgetStyle.Normal.SetImageSize(FVector2D(200, 200));
		KerunButton->WidgetStyle.Hovered.SetImageSize(FVector2D(210, 210));
		KerunButton->WidgetStyle.Pressed.SetImageSize(FVector2D(190, 190));
	}
	

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &ULobbyMenu::StartGame);
	}
	

	if (Lobby_StartButton)
	{
		Lobby_StartButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnLobbyStartButton);
	}

	ResetButtonSize();

	SidheRigelInstance = Cast<USidheRigelGameInstance>(GetGameInstance());
	if (SidheRigelInstance)
	{
		SidheRigelInstance->SetInputUI(TakeWidget());
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
	if (LobbyPlayerController->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("TEST In LobbyStartButton :: This Controller is %s"), *LobbyPlayerController->PlayerState->UniqueId->ToDebugString());
		
		LobbyPlayerController->OpenCharacterSelectMenu();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IN CLIENT :: Cant Access GameMode"));
	}
}

void ULobbyMenu::ResetButtonSize()
{
	ACMButton->WidgetStyle.Normal.OutlineSettings.Width = 1;
	ACMButton->WidgetStyle.Hovered.OutlineSettings.Width = 1;
	ACMButton->WidgetStyle.Pressed.OutlineSettings.Width = 1;

	BlackWizardButton->WidgetStyle.Normal.OutlineSettings.Width = 1;
	BlackWizardButton->WidgetStyle.Hovered.OutlineSettings.Width = 1;
	BlackWizardButton->WidgetStyle.Pressed.OutlineSettings.Width = 1;

	ColdButton->WidgetStyle.Normal.OutlineSettings.Width = 1;
	ColdButton->WidgetStyle.Hovered.OutlineSettings.Width = 1;
	ColdButton->WidgetStyle.Pressed.OutlineSettings.Width = 1;

	FairyWingButton->WidgetStyle.Normal.OutlineSettings.Width = 1;
	FairyWingButton->WidgetStyle.Hovered.OutlineSettings.Width = 1;
	FairyWingButton->WidgetStyle.Pressed.OutlineSettings.Width = 1;

	KerunButton->WidgetStyle.Normal.OutlineSettings.Width = 1;
	KerunButton->WidgetStyle.Hovered.OutlineSettings.Width = 1;
	KerunButton->WidgetStyle.Pressed.OutlineSettings.Width = 1;
}

void ULobbyMenu::RefreshPlayerList(TArray<class ALobbyPlayerController*> playerList)
{
	UE_LOG(LogTemp, Warning, TEXT("IN CLIENT :: playerList Num : %d"), playerList.Num());

	int32 idx = 0;
	for (auto player : playerList)
	{
		if (idx == 0)
		{
			Player_1->SetText(FText::FromString(player->PlayerState->UniqueId->ToDebugString()));
		}
		else if (idx == 1)
		{
			Player_2->SetText(FText::FromString(player->PlayerState->UniqueId->ToDebugString()));
		}
		idx++;
	}
}

void ULobbyMenu::SetCharacterACM()
{
	SidheRigelInstance->CharacterNum = ACM;
	ResetButtonSize();

	ACMButton->WidgetStyle.Normal.OutlineSettings.Width = 5;
	ACMButton->WidgetStyle.Hovered.OutlineSettings.Width = 5;
	ACMButton->WidgetStyle.Pressed.OutlineSettings.Width = 5;
}

void ULobbyMenu::SetCharacterBlackWizard()
{
	SidheRigelInstance->CharacterNum = BlackWizard;
	ResetButtonSize();

	BlackWizardButton->WidgetStyle.Normal.OutlineSettings.Width = 5;
	BlackWizardButton->WidgetStyle.Hovered.OutlineSettings.Width = 5;
	BlackWizardButton->WidgetStyle.Pressed.OutlineSettings.Width = 5;
}

void ULobbyMenu::SetCharacterCold()
{
	SidheRigelInstance->CharacterNum = Cold;
	ResetButtonSize();

	ColdButton->WidgetStyle.Normal.OutlineSettings.Width = 5;
	ColdButton->WidgetStyle.Hovered.OutlineSettings.Width = 5;
	ColdButton->WidgetStyle.Pressed.OutlineSettings.Width = 5;
}

void ULobbyMenu::SetCharacterFairyWing()
{
	SidheRigelInstance->CharacterNum = FairyWing;
	ResetButtonSize();

	FairyWingButton->WidgetStyle.Normal.OutlineSettings.Width = 5;
	FairyWingButton->WidgetStyle.Hovered.OutlineSettings.Width = 5;
	FairyWingButton->WidgetStyle.Pressed.OutlineSettings.Width = 5;
}

void ULobbyMenu::SetCharacterKerun()
{
	SidheRigelInstance->CharacterNum = Kerun;
	ResetButtonSize();

	KerunButton->WidgetStyle.Normal.OutlineSettings.Width = 5;
	KerunButton->WidgetStyle.Hovered.OutlineSettings.Width = 5;
	KerunButton->WidgetStyle.Pressed.OutlineSettings.Width = 5;
}

void ULobbyMenu::StartGame()
{
	LobbyPlayerController->Ready();
	
	//World->ServerTravel("/Game/Maps/TwistedDesert?listen");
	//World->ServerTravel("/Game/TopDown/Maps/TopDownMap?listen");
}
