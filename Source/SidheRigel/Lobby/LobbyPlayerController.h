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

	UFUNCTION()
		void Ready();

	UPROPERTY()
		bool isReady = false;

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Test_Func();

	UFUNCTION(Client, Reliable, WithValidation)
		void Client_Test_Func();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void Mult_Test_Func();

protected:
	virtual void BeginPlay() override;
};
