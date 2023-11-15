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
	auto exitPlayer = Cast<APlayerController>(Exiting);
	int32 idx = players.Find(exitPlayer);
	players.RemoveAt(idx);
	UIList.RemoveAt(idx);
}

void ALobbyGameMode::AddUIList(ULobbyMenu* UI)
{
	UIList.Add(UI);

	/*
	if (UIList.Num() == 2)
	{
		for (auto& UI : UIList)
		{
			UI->OpenCharacterSelectMenu();
		}
	}
	*/
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

void ALobbyGameMode::Ready()
{
	ReadyCount++;
	if (ReadyCount == players.Num())
	{
		UWorld* World = GetWorld();
		if (World == nullptr) return;

		World->ServerTravel("/Game/Maps/TrainingRoom?listen");
	}
}
