// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathTime.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UDeathTime : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized();

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimeText;

	void SetDeathCoolDown(float CurrentCoolDown);
	void ClearDeathCoolDown();
};
