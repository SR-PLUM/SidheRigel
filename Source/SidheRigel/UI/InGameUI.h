// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Enum/E_Character.h"
#include "InGameUI.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UInGameUI : public UUserWidget
{
	GENERATED_BODY()

public:
	/*virtual void NativeConstruct();*/


	UPROPERTY(meta = (BindWidget))
		class UCharacterStatus* CharacterStatus;

	UPROPERTY(meta = (BindWidget))
		class UCombatSummary* CombatSummary;

	UPROPERTY()
		class USidheRigelGameInstance* GameInstanceRef;

	UPROPERTY()
		class ASidheRigelCharacter* CharacterRef;

	
	void InitCharacterData(class USidheRigelGameInstance* GameInstance);

};
