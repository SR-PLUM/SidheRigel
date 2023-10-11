// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPUI.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UHPUI : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HPBar;

	void InitHPBar();
	void SetHPBar(float Percentage);
	void SetEnemyColor();
	void SetUIVisibility(bool Flag);

};
