// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverUI.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UGameOverUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized();

public:
	UPROPERTY(meta = (BindWidget))
		class UImage* GameOverImage;

	void SetGameOverImage(bool flag);

private:

	FString WinImage = TEXT("/Game/Heros/ACM/Icons/Icon_ACMWSkill");
	FString LoseImage = TEXT("/Game/Heros/ACM/Icons/Icon_ACMWSkill");

	UFUNCTION()
		UTexture2D* CastPathToTexture2D(FString Path);

};
