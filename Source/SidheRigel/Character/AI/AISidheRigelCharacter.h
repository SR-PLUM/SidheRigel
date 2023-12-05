// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "AISidheRigelCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API AAISidheRigelCharacter : public ASidheRigelCharacter
{
	GENERATED_BODY()
	
public:
	void SetTeam(E_Team setTeam);
};
