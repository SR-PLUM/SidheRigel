// Copyright Epic Games, Inc. All Rights Reserved.

#include "SidheRigelGameMode.h"
#include "SidheRigelPlayerController.h"
#include "SidheRigelCharacter.h"
#include "SidheRigelGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "SidheRigelCamera.h"

ASidheRigelGameMode::ASidheRigelGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ASidheRigelPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APawn> ACMBPClass(TEXT("/Game/Heros/ACM/BP_ACMCharacter"));
	if (ACMBPClass.Class != nullptr)
	{
		CharacterPath.Add("ACM", ACMBPClass.Class);
	}

	static ConstructorHelpers::FClassFinder<APawn> BlackWizardBPClass(TEXT("/Game/Heros/BlackWizard/BP_BlackWizardCharacter"));
	if (BlackWizardBPClass.Class != nullptr)
	{
		CharacterPath.Add("BlackWizard", BlackWizardBPClass.Class);
	}

	static ConstructorHelpers::FClassFinder<APawn> ColdBPClass(TEXT("/Game/Heros/Cold/BP_ColdCharacter"));
	if (ColdBPClass.Class != nullptr)
	{
		CharacterPath.Add("Cold", ColdBPClass.Class);
	}

	static ConstructorHelpers::FClassFinder<APawn> FairyWingBPClass(TEXT("/Game/Heros/FairyWIng/BP_FairyWingCharacter"));
	if (FairyWingBPClass.Class != nullptr)
	{
		CharacterPath.Add("FairyWing", FairyWingBPClass.Class);
	}

	static ConstructorHelpers::FClassFinder<APawn> KerunBPClass(TEXT("/Game/Heros/Kerun/BP_KerunCharacter"));
	if (KerunBPClass.Class != nullptr)
	{
		CharacterPath.Add("Kerun", KerunBPClass.Class);
	}

	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

	for (auto playerStart : PlayerStarts)
	{
		auto playerStartTag = Cast<APlayerStart>(playerStart)->PlayerStartTag;
		if (playerStartTag.ToString() == "None")
		{
			nonePlayerStarts.Add(playerStart);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Else Player STARTS :: %s"), *playerStartTag.ToString())
		}
	}
}

void ASidheRigelGameMode::InitGameState()
{
	Super::InitGameState();

	UGameInstance* Instance = GetGameInstance();

	USidheRigelGameInstance* InGameInstance = Cast<USidheRigelGameInstance>(Instance);
	if (InGameInstance)
	{
		switch (InGameInstance->CharacterNum)
		{
		case 1:
			if (CharacterPath.Contains("ACM"))
			{
				DefaultPawnClass = CharacterPath["ACM"];
			}
			break;
		case 2:
			if (CharacterPath.Contains("BlackWizard"))
			{
				DefaultPawnClass = CharacterPath["BlackWizard"];
			}
			break;
		case 3:
			if (CharacterPath.Contains("Cold"))
			{
				DefaultPawnClass = CharacterPath["Cold"];
			}
			break;
		case 4:
			if (CharacterPath.Contains("FairyWing"))
			{
				DefaultPawnClass = CharacterPath["FairyWing"];
			}
			break;
		case 5:
			if (CharacterPath.Contains("Kerun"))
			{
				DefaultPawnClass = CharacterPath["Kerun"];
			}
			break;
		default:
			break;
		}
	}
}

UClass* ASidheRigelGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	ASidheRigelPlayerController* SRController = Cast<ASidheRigelPlayerController>(InController);
	if (SRController)
	{
		return SRController->GetPlayerPawnClass();
	}

	return DefaultPawnClass;
}

AActor* ASidheRigelGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	AActor* SelectedPlayerStart;

	SelectedPlayerStart = nonePlayerStarts[0];

	nonePlayerStarts.RemoveAt(0);

	return SelectedPlayerStart;
}

void ASidheRigelGameMode::GameOver(E_Team team)
{

	ASidheRigelPlayerController* Controller = Cast<ASidheRigelPlayerController>(GetWorld()->GetFirstPlayerController());

	Controller->myCharacter->GameOverEffect(team);

	UE_LOG(LogTemp, Error, TEXT("Game Over"));

	//Move to Lobby
	//GetWorld()->GetTimerManager()->SetTimer


}

void ASidheRigelGameMode::BeginPlay()
{
	Super::BeginPlay();
}

