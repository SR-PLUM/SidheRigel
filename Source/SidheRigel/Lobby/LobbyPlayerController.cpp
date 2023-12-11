// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

#include "SidheRigel/Enum/E_Team.h"
#include "SidheRigel/SidheRigelGameInstance.h"
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

	bReplicates = true;
}

void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerController, myTeam);
	DOREPLIFETIME(ALobbyPlayerController, isReady);
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
	if (!isReady)
	{
		isReady = true;

		SetGameInstanceTeam(myTeam);

		auto currentGM = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (currentGM)
		{
			currentGM->Ready();
		}
	}
}

void ALobbyPlayerController::OpenCharacterSelectMenu_Implementation()
{
	LobbyUI->OpenCharacterSelectMenu();
}

void ALobbyPlayerController::SetPlayerControllerTeam_Implementation(E_Team setTeam)
{
	UE_LOG(LogTemp, Error, TEXT("TEST :: This Controller is %s"), *GetName());

	myTeam = setTeam;
}

void ALobbyPlayerController::SetGameInstanceTeam_Implementation(E_Team setGITeam)
{
	auto SRGameInstance = Cast<USidheRigelGameInstance>(GetGameInstance());
	if (SRGameInstance)
	{
		SRGameInstance->myTeam = setGITeam;
	}
}

void ALobbyPlayerController::IsCharacterSelected_Implementation()
{
	auto SRGameInstance = Cast<USidheRigelGameInstance>(GetGameInstance());
	if (SRGameInstance)
	{
		if (SRGameInstance->CharacterNum != E_Character::Character_Null)
			Ready();
	}
	//return false;
}

void ALobbyPlayerController::BeginPlay()
{
	FInputModeUIOnly InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	bShowMouseCursor = true;
	SetInputMode(InputModeData);
}
