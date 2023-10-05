// Fill out your copyright notice in the Description page of Project Settings.


#include "StatSummary.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UStatSummary::InitStat()
{
	Level->SetText(FText::FromString(FString::FromInt(1)));
	HPBar->SetPercent(1.f);
	ExprienceBar->SetPercent(1.f);
}

void UStatSummary::SetLevel(int32 CurrentLevel)
{
	Level->SetText(FText::FromString(FString::FromInt(CurrentLevel)));
}

void UStatSummary::SetHPBar(float Percentage)
{
	HPBar->SetPercent(Percentage);
}

void UStatSummary::SetExpBar(float Percentage)
{
	ExprienceBar->SetPercent(Percentage);
}
