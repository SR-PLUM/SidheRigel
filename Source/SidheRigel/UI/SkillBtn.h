// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillBtn.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API USkillBtn : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized();


public:
	UPROPERTY(meta = (BindWidget))
		class UButton* SkillBtn;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CountText;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* CoolDownBar;
};
