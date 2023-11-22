// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/*USTRUCT()
struct FPlayerInfo
{
	GENERATED_USTRUCT_BODY()

public:
	APlayerController* playerController;
	FText playerName;
	bool bReady;
	APlayerState* playerState;
};*/


UCLASS()
class SIDHERIGEL_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	ALobbyGameMode();

	void OpenCharacterSelectMenu();
	void RefreshPlayerText();
	void Ready();

	TArray<class ALobbyPlayerController*> players;

private:
	uint32 NumberOfPlayers = 0;
};
