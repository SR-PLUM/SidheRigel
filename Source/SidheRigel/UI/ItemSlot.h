// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
		class UImage* ItemImage;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ItemNum;

	UPROPERTY()
		FString ItemCode;

	UPROPERTY()
		FString ItemName;

	UPROPERTY()
		FString ItemDescription;

	void ItemInitialize();
};
