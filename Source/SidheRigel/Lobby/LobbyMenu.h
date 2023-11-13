// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../MainMenu/MenuWidget.h"
#include "LobbyMenu.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API ULobbyMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	
private:
	UPROPERTY(meta = (BindWidget))
		class UButton* ACMButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* BlackWizardButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ColdButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* FairyWingButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* KerunButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* StartGameButton;

	USidheRigelGameInstance* SidheRigelInstance;

	UFUNCTION()
		void SetCharacterACM();
	UFUNCTION()
		void SetCharacterBlackWizard();
	UFUNCTION()
		void SetCharacterCold();
	UFUNCTION()
		void SetCharacterFairyWing();
	UFUNCTION()
		void SetCharacterKerun();

	UFUNCTION()
		void StartGame();
};
