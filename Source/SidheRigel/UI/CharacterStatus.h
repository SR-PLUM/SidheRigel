// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Enum/E_Character.h"
#include "../Enum/E_SkillState.h"
#include "CharacterStatus.generated.h"


/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UCharacterStatus : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
		virtual bool Initialize() override;

protected:
	virtual void NativeOnInitialized();

public:

	UPROPERTY(meta = (BindWidget))
		class UImage* CharacterImage;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentLevelText;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* LevelProgress;

	TMap<E_SkillState, class USkillBtn*> SkillButtons;

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

public :
	UPROPERTY()
		class ASidheRigelCharacter* CharacterRef;

	void InitCharacterStatus(class ASidheRigelCharacter* Character);
	void InitIconImage(E_Character Character);

	void UpdateLevel();
	void UpdateHP();
	void UpdateMP();
	void UpdateMoney();



	UFUNCTION()
	UTexture2D* CastPathToTexture2D(FString Path);



	//IconPath
	TMap<E_Character, FString> CharacterIconPath = {
		TPair<E_Character, FString>(E_Character::ACM, TEXT("/Game/Heros/ACM/Icons/Icon_ACM")),
		TPair<E_Character, FString>(E_Character::BlackWizard, TEXT("/Game/Heros/BlackWizard/Icons/Icon_BlackWizard")),
		TPair<E_Character, FString>(E_Character::Cold, TEXT("/Game/Heros/Cold/Icons/Icon_Cold")),
		TPair<E_Character, FString>(E_Character::FairyWing, TEXT("/Game/Heros/FairyWing/Icons/Icon_FairyWing")),
		TPair<E_Character, FString>(E_Character::Kerun, TEXT("/Game/Heros/Kerun/Icons/Icon_Kerun"))
	};

	TMap<E_Character, FString> QSkillIconPath = {
		TPair<E_Character, FString>(E_Character::ACM, TEXT("/Game/Heros/ACM/Icons/Icon_ACMQSkill")),
		TPair<E_Character, FString>(E_Character::BlackWizard, TEXT("/Game/Heros/BlackWizard/Icons/Icon_BlackWizardQSkill")),
		TPair<E_Character, FString>(E_Character::Cold, TEXT("/Game/Heros/Cold/Icons/Icon_ColdQSkill")),
		TPair<E_Character, FString>(E_Character::FairyWing, TEXT("/Game/Heros/FairyWing/Icons/Icon_FairyWingQSkill")),
		TPair<E_Character, FString>(E_Character::Kerun, TEXT("/Game/Heros/Kerun/Icons/Icon_KerunQSkill"))
	};

	TMap<E_Character, FString> WSkillIconPath = {
		TPair<E_Character, FString>(E_Character::ACM, TEXT("/Game/Heros/ACM/Icons/Icon_ACMWSkill")),
		TPair<E_Character, FString>(E_Character::BlackWizard, TEXT("/Game/Heros/BlackWizard/Icons/Icon_BlackWizardWSkill")),
		TPair<E_Character, FString>(E_Character::Cold, TEXT("/Game/Heros/Cold/Icons/Icon_ColdWSkill")),
		TPair<E_Character, FString>(E_Character::FairyWing, TEXT("/Game/Heros/FairyWing/Icons/Icon_FairyWingWSkill")),
		TPair<E_Character, FString>(E_Character::Kerun, TEXT("/Game/Heros/Kerun/Icons/Icon_KerunWSkill"))
	};

	TMap<E_Character, FString> ESkillIconPath = {
		TPair<E_Character, FString>(E_Character::ACM, TEXT("/Game/Heros/ACM/Icons/Icon_ACMESkill")),
		TPair<E_Character, FString>(E_Character::BlackWizard, TEXT("/Game/Heros/BlackWizard/Icons/Icon_BlackWizardESkill")),
		TPair<E_Character, FString>(E_Character::Cold, TEXT("/Game/Heros/Cold/Icons/Icon_ColdESkill")),
		TPair<E_Character, FString>(E_Character::FairyWing, TEXT("/Game/Heros/FairyWing/Icons/Icon_FairyWingESkill")),
		TPair<E_Character, FString>(E_Character::Kerun, TEXT("/Game/Heros/Kerun/Icons/Icon_KerunESkill"))
	};

	TMap<E_Character, FString> RSkillIconPath = {
		TPair<E_Character, FString>(E_Character::ACM, TEXT("/Game/Heros/ACM/Icons/Icon_ACMRSkill")),
		TPair<E_Character, FString>(E_Character::BlackWizard, TEXT("/Game/Heros/BlackWizard/Icons/Icon_BlackWizardRSkill")),
		TPair<E_Character, FString>(E_Character::Cold, TEXT("/Game/Heros/Cold/Icons/Icon_ColdRSkill")),
		TPair<E_Character, FString>(E_Character::FairyWing, TEXT("/Game/Heros/FairyWing/Icons/Icon_FairyWingRSkill")),
		TPair<E_Character, FString>(E_Character::Kerun, TEXT("/Game/Heros/Kerun/Icons/Icon_KerunRSkill"))
	};
	
	FString BSkillIconPath = TEXT("/Game/UIBlueprints/InGameUI/WhiteRecall");
};
