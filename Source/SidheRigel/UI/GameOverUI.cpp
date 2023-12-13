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

	FLinearColor tmp = GameOverImage->Brush.TintColor.GetSpecifiedColor();
	tmp.R = 1.f;
	GameOverImage->SetBrushTintColor(tmp);

	GameOverImage->SetVisibility(ESlateVisibility::Visible);
	GameOverImage->SetBrushFromTexture(CastPathToTexture2D(ImagePath));
	
	UE_LOG(LogTemp, Error, TEXT("GameOverImage :: %d") , flag);
}

UTexture2D* UGameOverUI::CastPathToTexture2D(FString Path)
{
	UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
	return Texture;
}