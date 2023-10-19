// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TalentItem.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UTalentItem : public UUserWidget
{
	GENERATED_BODY()

public:
	UTalentItem(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(meta = (BindWidget))
		class UButton* BtnTalentItem;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TalentNameText;

	UPROPERTY()
		TSubclassOf<class UUserWidget> TalentToolTipWidget;

	class UTalentToolTip* ToolTip;

	FString TalentDescription;

	int32 TalentLevel;
	int32 TalentIndex;

	UPROPERTY()
		class ASidheRigelCharacter* CharacterRef;

public:
	UFUNCTION()
		void InitTalentItemRef(class ASidheRigelCharacter * Character, int32 Level, int32 Index);

	UFUNCTION()
		void InitTalentItemInfo();

	UFUNCTION()
		void OnClicked();

	void InitToolTip();
	
};
