// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackState.h"
#include "AttackStateMachine.h"

/**
 * 
 */
class SIDHERIGEL_API MoveToAttackState : public AttackState
{
	AttackStateMachine* attackStateMachine;

public:
	MoveToAttackState(AttackStateMachine* attackStateMachine, ASidheRigelCharacter* myPawn);
	~MoveToAttackState();

public:
	void Run();
};
