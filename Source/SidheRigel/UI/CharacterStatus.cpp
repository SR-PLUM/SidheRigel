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

	SkillButtons.Add(E_SkillState::Q_Ready, Cast<USkillBtn>(GetWidgetFromName(TEXT("QSkillBtn"))));
	SkillButtons.Add(E_SkillState::W_Ready, Cast<USkillBtn>(GetWidgetFromName(TEXT("WSkillBtn"))));
	SkillButtons.Add(E_SkillState::E_Ready, Cast<USkillBtn>(GetWidgetFromName(TEXT("ESkillBtn"))));
	SkillButtons.Add(E_SkillState::R_Ready, Cast<USkillBtn>(GetWidgetFromName(TEXT("RSkillBtn"))));
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

		SkillButtons[Q_Ready]->SkillBtn->WidgetStyle.Normal.SetResourceObject(CastPathToTexture2D(QSkillIconPath[Character]));
		SkillButtons[W_Ready]->SkillBtn->WidgetStyle.Normal.SetResourceObject(CastPathToTexture2D(WSkillIconPath[Character]));
		SkillButtons[E_Ready]->SkillBtn->WidgetStyle.Normal.SetResourceObject(CastPathToTexture2D(ESkillIconPath[Character]));
		SkillButtons[R_Ready]->SkillBtn->WidgetStyle.Normal.SetResourceObject(CastPathToTexture2D(RSkillIconPath[Character]));
		Btn_BSkill->WidgetStyle.Normal.SetResourceObject(CastPathToTexture2D(BSkillIconPath));

		SkillButtons[Q_Ready]->SkillBtn->WidgetStyle.Hovered.SetResourceObject(CastPathToTexture2D(QSkillIconPath[Character]));
		SkillButtons[W_Ready]->SkillBtn->WidgetStyle.Hovered.SetResourceObject(CastPathToTexture2D(WSkillIconPath[Character]));
		SkillButtons[E_Ready]->SkillBtn->WidgetStyle.Hovered.SetResourceObject(CastPathToTexture2D(ESkillIconPath[Character]));
		SkillButtons[R_Ready]->SkillBtn->WidgetStyle.Hovered.SetResourceObject(CastPathToTexture2D(RSkillIconPath[Character]));
		Btn_BSkill->WidgetStyle.Hovered.SetResourceObject(CastPathToTexture2D(BSkillIconPath));
		
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No!"));
	}

}

void UCharacterStatus::UpdateLevel()
{
	CurrentLevelText->SetText(FText::FromString(FString::FromInt(CharacterRef->GetCurrentLevel())));
	LevelProgress->SetPercent(float(CharacterRef->GetExp()) / CharacterRef->GetMaxExp());
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

/*
void UCharacterStatus::UpdateMoney()
{
	MoneyText->SetText(FText::FromString(FString::FromInt(CharacterRef->GetMoney())));
}
*/

UTexture2D* UCharacterStatus::CastPathToTexture2D(FString Path)
{
	UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
	return Texture;
}
