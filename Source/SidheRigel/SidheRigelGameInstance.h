// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainMenu/MenuInterface.h"
#include "SidheRigelGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API USidheRigelGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	USidheRigelGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
		void LoadMenu();

	UFUNCTION(BlueprintCallable)
		void LoadInGameMenu();

	UFUNCTION()
		void Host() override;

	UFUNCTION(Exec)
		void Join(const FString& Address) override;

	virtual void LoadMainMenu() override;

private:
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;

	const FString GameMapURL = "/Game/TopDown/Maps/TopDownMap?listen";
};
