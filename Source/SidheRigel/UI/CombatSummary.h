// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatSummary.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UCombatSummary : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* BlueScoreText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* RedScoreText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* KillCount;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DeathCount;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AssistCount;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MinionCount;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MinuteText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SecondText;
};
