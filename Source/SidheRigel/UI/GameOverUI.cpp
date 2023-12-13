// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverUI.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"

void UGameOverUI::NativeOnInitialized()
{
	GameOverImage->SetVisibility(ESlateVisibility::Hidden);

	FLinearColor tmp = GameOverImage->Brush.TintColor.GetSpecifiedColor();
	tmp.R = 0.f;
	GameOverImage->SetBrushTintColor(tmp);
}

void UGameOverUI::SetGameOverImage(bool flag)
{
	FString ImagePath;

	if (flag) ImagePath = WinImage;
	else ImagePath = LoseImage;

	GameOverImage->SetBrushFromTexture(CastPathToTexture2D(ImagePath));

	GameOverImage->SetVisibility(ESlateVisibility::Visible);
	FLinearColor tmp = GameOverImage->Brush.TintColor.GetSpecifiedColor();
	tmp.R = 1.f;
	GameOverImage->SetBrushTintColor(tmp);
}

UTexture2D* UGameOverUI::CastPathToTexture2D(FString Path)
{
	UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
	return Texture;
}