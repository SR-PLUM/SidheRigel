// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SidheRigelGameMode.generated.h"

UCLASS(minimalapi)
class ASidheRigelGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASidheRigelGameMode();

	virtual void InitGameState() override;

	UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	TMap<FString, TSubclassOf<APawn>> CharacterPath;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	UPROPERTY()
		TArray<AActor*> nonePlayerStarts;

protected:
	virtual void BeginPlay() override;
};



