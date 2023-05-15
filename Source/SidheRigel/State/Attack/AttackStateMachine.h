// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackState.h"

/**
 * 
 */
class SIDHERIGEL_API AttackStateMachine
{
public:
	AttackStateMachine(ASidheRigelCharacter* _MyPawn);
	~AttackStateMachine();

private:
	class AttackState* currentState;
public:
	AttackState* moveToAttackState;
	AttackState* doAttackState;
	AttackState* waitAttackState;
	

public:
	class ASidheRigelCharacter* MyPawn;
	void Run();
	void ChangeState(AttackState* nextState);

};
