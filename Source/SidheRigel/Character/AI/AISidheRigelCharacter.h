// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SidheRigel/Character/Kerun/KerunCharacter.h"
#include "AISidheRigelCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API AAISidheRigelCharacter : public AKerunCharacter
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay();

	void SetTeam(E_Team setTeam);
};
