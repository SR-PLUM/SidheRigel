// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/PlayerState.h"
#include "ThirdParty/Steamworks/Steamv151/sdk/public/steam/steam_api.h"

#include "SidheRigel/Lobby/LobbyMenu.h"
#include "LobbyPlayerController.h"

ALobbyGameMode::ALobbyGameMode()
{
	PlayerControllerClass = ALobbyPlayerController::StaticClass();
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	++NumberOfPlayers;
	auto lobbyPC = Cast<ALobbyPlayerController>(NewPlayer);
	players.Add(lobbyPC);

	UE_LOG(LogTemp,Warning,TEXT("IN SERVER :: PlayerNum : %d, NewPlayer : %s"), NumberOfPlayers, *NewPlayer->PlayerState->UniqueId->ToDebugString())
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	--NumberOfPlayers;
	auto exitPlayer = Cast<ALobbyPlayerController>(Exiting);
	int32 idx = players.Find(exitPlayer);
	players.Remove(exitPlayer);
}

void ALobbyGameMode::OpenCharacterSelectMenu(APlayerController* selector)
{
	if(HasAuthority() && players.Num() == 2)
	{
		for (auto& player : players)
		{
			if (player->LobbyUI)
			{
				player->LobbyUI->OpenCharacterSelectMenu();
			}
		}
	}
}

void ALobbyGameMode::RefreshPlayerText()
{
	for (auto& player : players)
	{
		if (player->LobbyUI)
		{
			player->LobbyUI->RefreshPlayerList(players);
		}
	}
}

void ALobbyGameMode::Ready()
{
	ReadyCount++;
	if (ReadyCount == players.Num() && HasAuthority())
	{
		UWorld* World = GetWorld();
		if (World == nullptr) return;

		World->ServerTravel("/Game/Maps/TrainingRoom?listen");
	}
}
