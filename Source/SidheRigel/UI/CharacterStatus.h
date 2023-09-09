// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStatus.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UCharacterStatus : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
		class UImage* CharacterImage;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentLevelText;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* LevelProgress;

	//Skill Button
	UPROPERTY(meta = (BindWidget))
		class UButton* Btn_QSkill;

	UPROPERTY(meta = (BindWidget))
		class UButton* Btn_WSkill;

	UPROPERTY(meta = (BindWidget))
		class UButton* Btn_ESkill;

	UPROPERTY(meta = (BindWidget))
		class UButton* Btn_RSkill;

	UPROPERTY(meta = (BindWidget))
		class UButton* Btn_BSkill;

	//HP, MP
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HPBar;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentHPText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxHpText;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* MPBar;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentMPText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxMpText;

	//ItemSlot
	UPROPERTY(meta = (BindWidget))
		class UItemSlot* ItemSlot1;

	UPROPERTY(meta = (BindWidget))
		class UItemSlot* ItemSlot2;

	UPROPERTY(meta = (BindWidget))
		class UItemSlot* ItemSlot3;

	UPROPERTY(meta = (BindWidget))
		class UItemSlot* ItemSlot4;

	UPROPERTY(meta = (BindWidget))
		class UItemSlot* ItemSlot5;

	UPROPERTY(meta = (BindWidget))
		class UItemSlot* ItemSlot6;

	//Shop
	UPROPERTY(meta = (BindWidget))
		class UButton* Btn_Shop;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MoneyText;
	
};
