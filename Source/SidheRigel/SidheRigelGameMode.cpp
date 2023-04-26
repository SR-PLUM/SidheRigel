// Copyright Epic Games, Inc. All Rights Reserved.

#include "SidheRigelGameMode.h"
#include "SidheRigelPlayerController.h"
#include "SidheRigelCharacter.h"
#include "UObject/ConstructorHelpers.h"

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

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}
