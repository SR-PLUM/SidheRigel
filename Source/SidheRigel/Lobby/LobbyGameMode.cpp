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
	if (lobbyPC)
	{
		players.Add(lobbyPC);

		UE_LOG(LogTemp, Warning, TEXT("IN SERVER :: PlayerNum : %d, NewPlayer : %s"), NumberOfPlayers, *NewPlayer->PlayerState->UniqueId->ToDebugString());

		for (auto& player : players)
		{
			if (player->LobbyUI)
			{
				player->LobbyUI->RefreshPlayerList(players);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("IN SERVER :: %s Lobby UI Is NULL"), *NewPlayer->PlayerState->UniqueId->ToDebugString());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IN SERVER :: %s PC Is Not LobbyPC"), *NewPlayer->PlayerState->UniqueId->ToDebugString());
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	--NumberOfPlayers;
	auto exitPlayer = Cast<ALobbyPlayerController>(Exiting);
	int32 idx = players.Find(exitPlayer);
	players.Remove(exitPlayer);
}

void ALobbyGameMode::OpenCharacterSelectMenu()
{
	if(HasAuthority())
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
	//게임 시작 버튼 따로 만들어야할듯
	if (HasAuthority())
	{
		int32 ReadyCount = 0;
		for (auto& player : players)
		{
			if (player->isReady)
			{
				ReadyCount++;
				UE_LOG(LogTemp, Warning, TEXT("IN SERVER :: %s PC Is Ready State"), *player->PlayerState->UniqueId->ToDebugString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("IN SERVER :: %s PC Is Ready State"), *player->PlayerState->UniqueId->ToDebugString());
			}
		}

		if (ReadyCount == NumberOfPlayers)
		{
			UWorld* World = GetWorld();
			if (World == nullptr) return;

			World->ServerTravel("/Game/Maps/TrainingRoom?listen");
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("IN SERVER :: Not Enough ReadyCount, Current ReadyCount : %d"), ReadyCount);
		}
	}
}
