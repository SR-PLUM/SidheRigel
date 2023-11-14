// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	++NumberOfPlayers;
	players.Add(NewPlayer);

	UE_LOG(LogTemp, Warning, TEXT("LOGIN :: %s"), *NewPlayer->PlayerState->UniqueId->ToString());
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	--NumberOfPlayers;

	UE_LOG(LogTemp, Warning, TEXT("LOGIN :: %s"), *Exiting->GetName());
}
