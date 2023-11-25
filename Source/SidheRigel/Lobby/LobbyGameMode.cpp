// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/PlayerState.h"
#include "ThirdParty/Steamworks/Steamv151/sdk/public/steam/steam_api.h"
#include "Net/UnrealNetwork.h"

#include "SidheRigel/Lobby/LobbyMenu.h"
#include "LobbyPlayerController.h"

ALobbyGameMode::ALobbyGameMode()
{
	PlayerControllerClass = ALobbyPlayerController::StaticClass();
}

void ALobbyGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameMode, players);
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	++NumberOfPlayers;
	auto lobbyPC = Cast<ALobbyPlayerController>(NewPlayer);
	if (lobbyPC)
	{
		players.Add(lobbyPC);

		lobbyPC->Server_RefreshPlayerList(players);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	--NumberOfPlayers;
	auto exitPlayer = Cast<ALobbyPlayerController>(Exiting);
	int32 idx = players.Find(exitPlayer);
	players.Remove(exitPlayer);
}


void ALobbyGameMode::Ready(int32 readyCount)
{
	if (readyCount >= players.Num())
	{
		//게임 시작 버튼 따로 만들어야할듯
		UWorld* World = GetWorld();
		if (World == nullptr) return;

		World->ServerTravel("/Game/Maps/TrainingRoom?listen");
	}
}
