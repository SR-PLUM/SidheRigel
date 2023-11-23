// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"

#include "SidheRigel/Lobby/LobbyMenu.h"
#include "LobbyGameMode.h"

ALobbyPlayerController::ALobbyPlayerController()
{
	FSoftClassPath LobbyMenuBP(TEXT("WidgetBlueprint'/Game/UIBlueprints/WBP_LobbyMenu.WBP_LobbyMenu_C'"));
	if (UClass* LobbyMenuClass = LobbyMenuBP.TryLoadClass<UUserWidget>())
	{
		LobbyUI = CreateWidget<ULobbyMenu>(GetWorld(), LobbyMenuClass);
		if (LobbyUI)
		{
			LobbyUI->AddToViewport();

			LobbyUI->LobbyPlayerController = this;
		}
	}
}

void ALobbyPlayerController::RefreshPlayerList_Implementation(const TArray<class ALobbyPlayerController*>& playerList)
{
	UE_LOG(LogTemp, Warning, TEXT("TEST :: This Controller is %s"), *PlayerState->UniqueId->ToDebugString());
	LobbyUI->RefreshPlayerList(playerList);
}

void ALobbyPlayerController::Ready_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("TEST :: This Controller is %s"), *PlayerState->UniqueId->ToDebugString());
	if (!isReady)
	{
		isReady = true;
		UE_LOG(LogTemp, Warning, TEXT("IN CLIENT :: State Is Ready"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IN CLIENT :: Already Ready State"));
	}

	auto currentGM = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (currentGM)
	{
		UE_LOG(LogTemp, Warning, TEXT("IN CLIENT :: Try Access LobbyGameMode"));
		currentGM->Ready();
	}
}

void ALobbyPlayerController::OpenCharacterSelectMenu_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("TEST :: This Controller is %s"), *PlayerState->UniqueId->ToDebugString());
	LobbyUI->OpenCharacterSelectMenu();
}

void ALobbyPlayerController::BeginPlay()
{
	
}
