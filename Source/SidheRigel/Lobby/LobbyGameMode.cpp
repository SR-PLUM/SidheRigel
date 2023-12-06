// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/PlayerState.h"
#include "ThirdParty/Steamworks/Steamv151/sdk/public/steam/steam_api.h"
#include "Net/UnrealNetwork.h"
#include "Components/WidgetComponent.h"
#include "LoadingScreen.h"

#include "SidheRigel/SidheRigelGameInstance.h"
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
		FPlayerInfo tempPlayerInfo;
		if (NumberOfPlayers % 2 == 1)
		{
			tempPlayerInfo.team = E_Team::Blue;
		}
		else
		{
			tempPlayerInfo.team = E_Team::Red;
		}

		tempPlayerInfo.playerController = lobbyPC;

		players.Add(tempPlayerInfo);

		TArray<ALobbyPlayerController*> controllers;
		for (auto player : players)
		{
			controllers.Add(player.playerController);
		}

		lobbyPC->SetPlayerControllerTeam(tempPlayerInfo.team);

		lobbyPC->Server_RefreshPlayerList(controllers);
		
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	--NumberOfPlayers;
	auto exitPlayer = Cast<ALobbyPlayerController>(Exiting);
	
	for (int32 i = 0; i < players.Num(); i++)
	{
		if (players[i].playerController == exitPlayer)
		{
			players.RemoveAt(i);
			break;
		}
	}
}


void ALobbyGameMode::Ready()
{
	readyCount++;

	if (readyCount >= players.Num())
	{
		if (readyCount == 1)
		{
			auto SRInstance =  Cast<USidheRigelGameInstance>(players[0].playerController->GetGameInstance());
			SRInstance->bIsSingleGame = true;
		}

		UWorld* World = GetWorld();
		if (World == nullptr) return;

		players[0].playerController->LobbyUI->OpenLoadingScreen();

		FTimerHandle TimerHandler;
		World->GetTimerManager().SetTimer(TimerHandler, [&]()
			{
				ChangeServer();
			}, 1, false);

		
	}
}

void ALobbyGameMode::ChangeServer()
{
	UWorld* World = GetWorld();

	if (World)
	{
		World->ServerTravel("/Game/Maps/TrainingRoom?listen");
	}	
}
