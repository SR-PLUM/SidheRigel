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

public:
	UFUNCTION(Client, Reliable)
		void RefreshPlayerList(const TArray<class ALobbyPlayerController*>& playerList);
	UFUNCTION(NetMulticast, Reliable)
		void OpenCharacterSelectMenu();
	UFUNCTION(Server, Reliable)
		void Ready();

protected:
	virtual void BeginPlay() override;
};
