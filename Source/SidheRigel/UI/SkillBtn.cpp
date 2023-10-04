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
