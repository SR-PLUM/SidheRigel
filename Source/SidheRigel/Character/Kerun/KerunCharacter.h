// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "KerunCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API AKerunCharacter : public ASidheRigelCharacter
{
	GENERATED_BODY()

public:
	AKerunCharacter();

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
