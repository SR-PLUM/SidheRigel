// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackStateMachine.h"
#include "MoveToAttackState.h"
#include "DoAttackState.h"
#include "WaitAttackState.h"
#include "SidheRigel/SidheRigelCharacter.h"

AttackStateMachine::AttackStateMachine(ASidheRigelCharacter* _MyPawn)
{
	this->moveToAttackState = new MoveToAttackState(this, _MyPawn);
	this->doAttackState = new DoAttackState(this, _MyPawn);
	this->waitAttackState = new WaitAttackState(this, _MyPawn);

	this->currentState = moveToAttackState;
	this->MyPawn = _MyPawn;
}

AttackStateMachine::~AttackStateMachine()
{
}

void AttackStateMachine::Run()
{
	currentState->Run();
}

void AttackStateMachine::ChangeState(AttackState* nextState)
{
	this->currentState = nextState;
}
