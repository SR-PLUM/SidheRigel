// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/PlayerState.h"

#include "SidheRigel/Lobby/LobbyMenu.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	++NumberOfPlayers;
	players.Add(NewPlayer);
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	--NumberOfPlayers;

	UE_LOG(LogTemp, Warning, TEXT("LogOut :: %s"), *Exiting->GetName());
}

void ALobbyGameMode::AddUIList(ULobbyMenu* UI)
{
	UIList.Add(UI);
}

void ALobbyGameMode::OpenCharacterSelectMenu(APlayerController* selector)
{
	if (selector == players[0])
	{
		for (auto& UI : UIList)
		{
			UI->OpenCharacterSelectMenu();
		}
	}
}

void ALobbyGameMode::RefreshPlayerText()
{
	for (auto& UI : UIList)
	{
		UI->RefreshPlayerList(players);
	}
}
