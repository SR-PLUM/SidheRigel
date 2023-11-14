// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
USTRUCT()
struct FServerData
{
	GENERATED_BODY()

public:
	FString Name;
	uint16 CurrentPlayer;
	uint16 MaxPlayer;
	FString HostUsername;
};

UCLASS()
class SIDHERIGEL_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerNames);

	void SelectIndex(uint32 Index);

	UFUNCTION()
		void OpenLobbyMenu();

	void RefreshPlayerList();

protected:
	virtual bool Initialize() override;

private:
	TSubclassOf<class UUserWidget> ServerRowClass;

	UPROPERTY(meta = (BindWidget))
		class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
		class UButton* Join_CancelButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* Join_JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
		class UEditableText* ServerHostName;

	UPROPERTY(meta = (BindWidget))
		class UButton* Host_HostButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* Host_CancelButton;

	UPROPERTY(meta = (BindWidget))
		class UWidget* LobbyMenu;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Player_1;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Player_2;

	UPROPERTY(meta = (BindWidget))
		class UButton* Lobby_StartButton;

	TOptional<uint32> SelectedIndex;

	UFUNCTION()
		void HostServer();

	UFUNCTION()
		void JoinServer();

	UFUNCTION()
		void OpenJoinMenu();

	UFUNCTION()
		void OpenMainMenu();

	UFUNCTION()
		void OpenHostMenu();

	UFUNCTION()
		void QuitPressed();
	
	void UpdateChildren();
};
