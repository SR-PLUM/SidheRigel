// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatus.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#include "SkillBtn.h"

#include "Styling/SlateBrush.h"
#include "SidheRigel/SidheRigelCharacter.h"

bool UCharacterStatus::Initialize()
{
	return Super::Initialize();
}

void UCharacterStatus::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//USkillBtn* Tmp = Cast<USkillBtn>(GetWidgetFromName(TEXT("Btn_QSkill")));

	//SkillButtons.Add(E_SkillState::Q_Ready, Tmp);
	
	//
}

void UCharacterStatus::InitCharacterStatus(ASidheRigelCharacter* Character)
{
	CharacterRef = Character;

	UpdateLevel();

	UpdateHP();

	UpdateMP();
}

void UCharacterStatus::InitIconImage(E_Character Character)
{
	if (CharacterImage != nullptr)
	{
		CharacterImage->SetBrushFromTexture(CastPathToTexture2D(CharacterIconPath[Character]));

		Btn_QSkill->WidgetStyle.Normal.SetResourceObject(CastPathToTexture2D(QSkillIconPath[Character]));
		Btn_WSkill->WidgetStyle.Normal.SetResourceObject(CastPathToTexture2D(WSkillIconPath[Character]));
		Btn_ESkill->WidgetStyle.Normal.SetResourceObject(CastPathToTexture2D(ESkillIconPath[Character]));
		Btn_RSkill->WidgetStyle.Normal.SetResourceObject(CastPathToTexture2D(RSkillIconPath[Character]));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No!"));
	}

}

void UCharacterStatus::UpdateLevel()
{
	CurrentLevelText->SetText(FText::FromString(FString::FromInt(CharacterRef->GetCurrentLevel())));
	//TODO : Level Percentage
}

void UCharacterStatus::UpdateHP()
{
	MaxHpText->SetText(FText::FromString(FString::FromInt(CharacterRef->GetMaxHP())));
	CurrentHPText->SetText(FText::FromString(FString::FromInt(CharacterRef->GetCurrentHP())));
	HPBar->SetPercent(CharacterRef->GetCurrentHP() / CharacterRef->GetMaxHP());
}

void UCharacterStatus::UpdateMP()
{
	MaxMpText->SetText(FText::FromString(FString::FromInt(CharacterRef->GetMaxMP())));
	CurrentMPText->SetText(FText::FromString(FString::FromInt(CharacterRef->GetCurrentMP())));
	MPBar->SetPercent(CharacterRef->GetCurrentMP() / CharacterRef->GetMaxMP());
}

void UCharacterStatus::UpdateMoney()
{
	MoneyText->SetText(FText::FromString(FString::FromInt(CharacterRef->GetMoney())));
}

UTexture2D* UCharacterStatus::CastPathToTexture2D(FString Path)
{
	UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
	return Texture;
}
