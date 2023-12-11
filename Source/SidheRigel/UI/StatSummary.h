// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatSummary.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UStatSummary : public UUserWidget
{
	GENERATED_BODY()


public:

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Level;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HPBar;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* MPBar;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ExprienceBar;

	void InitStat();
	void SetLevel(int32 level);
	void SetHPBar(float Percentage);
	void SetMPBar(float Percentage);
	void SetExpBar(float Percentage);
	void SetEnemyColor();
};
