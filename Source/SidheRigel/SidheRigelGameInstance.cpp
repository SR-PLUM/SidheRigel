// Fill out your copyright notice in the Description page of Project Settings.


#include "SidheRigelGameInstance.h"

#include "OnlineSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"

#include "MainMenu/MainMenu.h"
#include "MainMenu/InGameMenu.h"
#include "MainMenu/MenuWidget.h"

const static FName SESSION_NAME = TEXT("My Session Game");

USidheRigelGameInstance::USidheRigelGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/UIBlueprints/WBP_Mainmenu"));
	if (MainMenuBPClass.Class == nullptr)
		return;

	MenuClass = MainMenuBPClass.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget>InGameMenuBPClass(TEXT("/Game/UIBlueprints/WBP_InGameMenu"));
	if (InGameMenuBPClass.Class == nullptr)
		return;

	InGameMenuClass = InGameMenuBPClass.Class;
}

void USidheRigelGameInstance::LoadMenu()
{
	if (MenuClass == nullptr)
		return;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (Menu == nullptr)
		return;

	Menu->Setup();

	Menu->SetMenuInterface(this);
}

void USidheRigelGameInstance::LoadInGameMenu()
{
	if (InGameMenuClass == nullptr) return;

	UInGameMenu* menu = CreateWidget<UInGameMenu>(this, InGameMenuClass);
	if (menu == nullptr) return;

	menu->Setup();

	menu->SetMenuInterface(this);
}

void USidheRigelGameInstance::Host()
{
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void USidheRigelGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	if (Menu != nullptr)
	{
		Menu->Teardown();
	}

	UE_LOG(LogTemp, Warning, TEXT("JOIN"));

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void USidheRigelGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PlayerController == nullptr) return;

	PlayerController->ClientTravel("/Game/Maps/Mainmenu", ETravelType::TRAVEL_Absolute);
}

void USidheRigelGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found no Subsystem"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Subsystem : %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &USidheRigelGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &USidheRigelGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &USidheRigelGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &USidheRigelGameInstance::OnJoinSessionComplete);
		}
	}
}

void USidheRigelGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = true;
		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void USidheRigelGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not Create Session"));
		return;
	}

	if (Menu != nullptr)
	{
		Menu->Teardown();
	}

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	World->ServerTravel(GameMapURL);
}

void USidheRigelGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void USidheRigelGameInstance::OnFindSessionComplete(bool Success)
{
	if (Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Finished Find Session"));

		TArray<FString> ServerNames;

		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Fount Session Name : %s"), *SearchResult.GetSessionIdStr());
			ServerNames.Add(SearchResult.GetSessionIdStr());
		}

		Menu->SetServerList(ServerNames);
	}
}

void USidheRigelGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get Connect String"));
		return;
	}

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PlayerController == nullptr) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void USidheRigelGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 6;
		SessionSettings.bShouldAdvertise = true;

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}
