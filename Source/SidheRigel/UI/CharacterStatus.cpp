// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatus.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Styling/SlateBrush.h"

bool UCharacterStatus::Initialize()
{
	return Super::Initialize();
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

UTexture2D* UCharacterStatus::CastPathToTexture2D(FString Path)
{
	UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
	return Texture;
}
