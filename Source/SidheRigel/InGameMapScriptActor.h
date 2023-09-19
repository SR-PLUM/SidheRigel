// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "InGameMapScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API AInGameMapScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	AInGameMapScriptActor();

	TSubclassOf<UUserWidget> InGameUIWidget;

	UPROPERTY(VisibleInstanceOnly, Category = "Widget")
		class UInGameUI* InGameUI;

protected:
	virtual void BeginPlay() override;
};
