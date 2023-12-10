// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreen.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API ULoadingScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Setup();
	void Teardown();
};
