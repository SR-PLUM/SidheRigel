// Fill out your copyright notice in the Description page of Project Settings.


#include "SidheRigelGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "MainMenu/MainMenu.h"

USidheRigelGameInstance::USidheRigelGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/UIBlueprints/WBP_Mainmenu"));
	if (MainMenuBPClass.Class == nullptr)
		return;

	MenuClass = MainMenuBPClass.Class;
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

	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("DEBUG")));
}

void USidheRigelGameInstance::Host()
{
	if (Menu != nullptr)
	{
		Menu->Teardown();
	}

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	World->ServerTravel(GameMapURL);
}

void USidheRigelGameInstance::Join(const FString& Address)
{
	if (Menu != nullptr)
	{
		Menu->Teardown();
	}

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PlayerController == nullptr) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}
