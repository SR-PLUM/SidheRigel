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

/*
APlayerController* ASidheRigelGameMode::SpawnPlayerController(ENetRole InRemoteRole, const FString& Options)
{
	
	// Make sure that our own player controller class will be used
	PlayerControllerClass = ASidheRigelPlayerController::StaticClass();
	// Call the super method: thats where the class will get instantiated and returned
	APlayerController* PlayerController = Super::SpawnPlayerController(InRemoteRole, Options);
	// The player controller class will be our player controller class (AMyPlayerController in this case) because of the line PlayerControllerClass = AMyPlayerController::StaticClass(); at the beginning of the method
	// We can safely cast the instantiated player controller class to our player controller class so that we can use our methods
	ASidheRigelPlayerController* MyPlayerController = CastChecked<ASidheRigelPlayerController>(PlayerController);
	// Get the world. The one will be used to instantiate our camera actor
	UWorld* World = GetWorld();
	// Instantiate our camera actor (actor of type AMyCamera in this case) at position (0, 0, 0) with rotation (0, 0, 0)
	AActor* CameraActor = World->SpawnActor(ASidheRigelCamera::StaticClass(), &FVector::ZeroVector, &FRotator::ZeroRotator);
	// We have just spawned AMyCamera and assigned it to the variable CameraActor. Since we know it is of type AMyCamera, we can safelty cast CameraActor variable to the AMyCamera class to be able to use its methods
	ASidheRigelCamera* MyCamera = CastChecked<ASidheRigelCamera>(CameraActor);
	// Pass the instantiated player controller to the camera so that it can read the rotation from one
	MyCamera->SetPlayerController(PlayerController);
	// Pass instantiated camera to the player controller so that it can be bound to the player the controller possesses
	MyPlayerController->SetCamera(MyCamera);
	// Return the instantiated player controller
	return PlayerController;
	
}
*/
