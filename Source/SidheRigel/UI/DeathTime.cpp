// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathTime.h"

#include "Components/TextBlock.h"

void UDeathTime::NativeOnInitialized()
{
	TimeText->SetVisibility(ESlateVisibility::Hidden);
}

void UDeathTime::SetDeathCoolDown(float CurrentCoolDown)
{
	TimeText->SetVisibility(ESlateVisibility::Visible);
	TimeText->SetText(FText::FromString(FString::FromInt(CurrentCoolDown)));
}

void UDeathTime::ClearDeathCoolDown()
{
	TimeText->SetVisibility(ESlateVisibility::Hidden);
	TimeText->SetText(FText::FromString(FString::FromInt(0)));
}