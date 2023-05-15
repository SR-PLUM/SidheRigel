// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackState.h"
#include "AttackStateMachine.h"

/**
 * 
 */
class SIDHERIGEL_API WaitAttackState : public AttackState
{
	AttackStateMachine* attackStateMachine;

public:
	WaitAttackState(AttackStateMachine* attackStateMachine, ASidheRigelCharacter* myPawn);
	~WaitAttackState();

public:
	void Run();
};
