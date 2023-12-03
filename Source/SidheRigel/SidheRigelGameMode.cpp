// Copyright Epic Games, Inc. All Rights Reserved.

#include "SidheRigelGameMode.h"
#include "SidheRigelPlayerController.h"
#include "SidheRigelCharacter.h"
#include "SidheRigelGameInstance.h"
#include "UObject/ConstructorHelpers.h"
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

