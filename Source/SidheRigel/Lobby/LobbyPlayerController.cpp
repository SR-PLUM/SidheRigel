// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

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

	FInputModeUIOnly InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	bShowMouseCursor = true;
	SetInputMode(InputModeData);
}

void ALobbyPlayerController::RefreshPlayerList_Implementation(const TArray<FText>& nameList)
{
	LobbyUI->RefreshPlayerList(nameList);
}

void ALobbyPlayerController::Server_RefreshPlayerList_Implementation(const TArray<class ALobbyPlayerController*>& playerList)
{
	TArray<FText> NameList;
	for (auto player : playerList)
	{
		NameList.Add(FText::FromString(player->PlayerState->UniqueId->ToDebugString()));
	}

	LobbyUI->RefreshPlayerList(NameList);
	RefreshPlayerList(NameList);
}

void ALobbyPlayerController::Ready_Implementation()
{
	readyCount++;
	
	auto currentGM = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (currentGM)
	{
		currentGM->Ready(readyCount);
	}
}

void ALobbyPlayerController::OpenCharacterSelectMenu_Implementation()
{
	LobbyUI->OpenCharacterSelectMenu();
}

void ALobbyPlayerController::Client_Ready()
{
	if (!isReady)
	{
		isReady = true;
		Ready();
	}
}

void ALobbyPlayerController::BeginPlay()
{
	
}
