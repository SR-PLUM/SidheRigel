// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"

#include "Kismet/GameplayStatics.h"

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

void ALobbyPlayerController::Ready()
{
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

void ALobbyPlayerController::Server_Test_Func_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("TEST :: Server_Test_Func"));
}

bool ALobbyPlayerController::Server_Test_Func_Validate()
{
	return true;
}

void ALobbyPlayerController::Client_Test_Func_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("TEST :: Client_Test_Func"));
}

bool ALobbyPlayerController::Client_Test_Func_Validate()
{
	return true;
}

void ALobbyPlayerController::Mult_Test_Func_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("TEST :: Mult_Test_Func"));
}

bool ALobbyPlayerController::Mult_Test_Func_Validate()
{
	return true;
}

void ALobbyPlayerController::BeginPlay()
{
	Server_Test_Func();

	Client_Test_Func();

	Mult_Test_Func();
}
