// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	++NumberOfPlayers;

	if (NumberOfPlayers >= 6)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reached Max Player"));
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	--NumberOfPlayers;
}
