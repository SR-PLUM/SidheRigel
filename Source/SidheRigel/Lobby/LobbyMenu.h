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
public:
	UFUNCTION()
		void OpenLobbyMenu();

	UFUNCTION()
		void OpenCharacterSelectMenu();

	UFUNCTION()
		void OnLobbyStartButton();

	void RefreshPlayerList(TArray<class APlayerController*> playerList);

protected:
	virtual bool Initialize() override;
	
private:
	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* LobbySwitcher;

	UPROPERTY(meta = (BindWidget))
		class UWidget* LobbyMenu;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Player_1;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Player_2;

	UPROPERTY(meta = (BindWidget))
		class UButton* Lobby_StartButton;

	UPROPERTY(meta = (BindWidget))
		class UWidget* CharacterSelectMenu;

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

	class USidheRigelGameInstance* SidheRigelInstance;
	class ALobbyGameMode* LobbyGameMode;

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
