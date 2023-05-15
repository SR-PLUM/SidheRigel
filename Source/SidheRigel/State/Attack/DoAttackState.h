// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackState.h"
#include "AttackStateMachine.h"

/**
 * 
 */
class SIDHERIGEL_API DoAttackState : public AttackState
{
	AttackStateMachine* attackStateMachine;

public:
	DoAttackState(AttackStateMachine* attackStateMachine, ASidheRigelCharacter* myPawn);
	~DoAttackState();

public:
	void Run();
};
