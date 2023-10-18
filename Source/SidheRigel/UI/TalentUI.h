// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TalentUI.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UTalentUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	TArray<class UTalentItem*> TalentItemList;

	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* TalentItemBox;

};
