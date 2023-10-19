// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TalentToolTip.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UTalentToolTip : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DescriptionText;
	
};
