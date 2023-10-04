// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBtn.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void USkillBtn::NativeOnInitialized()
{
	CoolDownBar->SetPercent(0.f);
	CountText->SetVisibility(ESlateVisibility::Hidden);
}

void USkillBtn::SetCoolDownProgress(float Percentage, float CurrentCoolDown)
{
	CountText->SetVisibility(ESlateVisibility::Visible);
	CoolDownBar->SetPercent(Percentage);
	CountText->SetText(FText::FromString(FString::FromInt(CurrentCoolDown)));
}

void USkillBtn::ClearCoolDownProgress()
{
	CoolDownBar->SetPercent(0.f);
	CountText->SetText(FText::FromString(FString::FromInt(0)));
	CountText->SetVisibility(ESlateVisibility::Hidden);
}


