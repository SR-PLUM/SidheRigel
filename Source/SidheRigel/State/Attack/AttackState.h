// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SidheRigel/SidheRigelCharacter.h"

/**
 * 
 */
class SIDHERIGEL_API AttackState
{
public:
	AttackState();
	~AttackState();

public:
	virtual void Run();
	class ASidheRigelCharacter* MyPawn;
};
