// Fill out your copyright notice in the Description page of Project Settings.


#include "HPUI.h"

#include "Components/ProgressBar.h"

void UHPUI::InitHPBar()
{
	HPBar->SetPercent(1.0f);
}

void UHPUI::SetHPBar(float Percentage)
{
	HPBar->SetPercent(Percentage);
}

void UHPUI::SetEnemyColor()
{
	HPBar->FillColorAndOpacity = FLinearColor(255.f, 0.f, 0.f, 1.f);
}

void UHPUI::SetUIVisibility(bool Flag)
{
	if (Flag)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}
