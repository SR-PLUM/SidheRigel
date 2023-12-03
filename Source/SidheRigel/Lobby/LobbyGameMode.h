// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

USTRUCT()
struct FPlayerInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
		class ALobbyPlayerController* playerController;
	UPROPERTY()
		TEnumAsByte<E_Team> team;
};


UCLASS()
class SIDHERIGEL_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	ALobbyGameMode();

	UFUNCTION()
		void Ready();

	UPROPERTY(replicated)
		TArray<FPlayerInfo> players;
	UPROPERTY()
		int32 readyCount = 0;

private:
	uint32 NumberOfPlayers = 0;
};
