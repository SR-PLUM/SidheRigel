// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	++NumberOfPlayers;
	players.Add(NewPlayer);

	UE_LOG(LogTemp, Warning, TEXT("LOGIN :: %s"), *NewPlayer->GetName());
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	--NumberOfPlayers;

	UE_LOG(LogTemp, Warning, TEXT("LOGIN :: %s"), *Exiting->GetName());
}
