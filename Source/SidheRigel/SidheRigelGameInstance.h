// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainMenu/MenuInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
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
		void Join(uint32 Index) override;

	virtual void LoadMainMenu() override;

	UFUNCTION()
		virtual void Init() override;

	virtual void RefreshServerList() override;

private:
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnSessionInviteReceived(const FUniqueNetId& UserId, const FUniqueNetId& FromId, const FString& AppId, const FOnlineSessionSearchResult& OnlineSessionSearchResult);
	void OnSessionUserInviteAccepted(bool bWasSuccessful, int ControllerId, TSharedPtr<const FUniqueNetId, ESPMode::Fast> UserId, const FOnlineSessionSearchResult& OnlineSessionSearchResult);

	void CreateSession();

	const FString GameMapURL = "/Game/TopDown/Maps/TopDownMap?listen";
};
