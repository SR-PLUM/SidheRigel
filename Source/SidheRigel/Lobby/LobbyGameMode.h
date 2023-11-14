// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	void AddUIList(class ULobbyMenu* UI);
	void OpenCharacterSelectMenu(APlayerController* selector);
	void RefreshPlayerText();

	TArray<APlayerController*> players;
	TArray<class ULobbyMenu*> UIList;

private:
	uint32 NumberOfPlayers = 0;
};
