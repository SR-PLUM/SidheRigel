// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ALobbyPlayerController();

	UPROPERTY()
		class ULobbyMenu* LobbyUI;
	UPROPERTY(replicated)
		bool isReady = false;
	UPROPERTY(replicated, EditAnyWhere)
		TEnumAsByte<E_Team> myTeam;

public:
	UFUNCTION(Client, Reliable)
		void RefreshPlayerList(const TArray<FText>& nameList);
	UFUNCTION(Server, Reliable)
		void Server_RefreshPlayerList(const TArray<class ALobbyPlayerController*>& playerList);
	UFUNCTION(NetMulticast, Reliable)
		void OpenCharacterSelectMenu();
	UFUNCTION(Server, Reliable)
		void Ready();
	UFUNCTION(Client, Reliable)
		void SetGameInstanceTeam(E_Team setGITeam);
	UFUNCTION(server, reliable)
		void SetPlayerControllerTeam(E_Team setTeam);

protected:
	virtual void BeginPlay() override;
};
