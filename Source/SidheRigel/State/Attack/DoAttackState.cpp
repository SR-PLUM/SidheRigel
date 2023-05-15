// Fill out your copyright notice in the Description page of Project Settings.


#include "DoAttackState.h"

DoAttackState::DoAttackState(AttackStateMachine* attackStateMachine, ASidheRigelCharacter* myPawn)
{
	this->attackStateMachine = attackStateMachine;
	MyPawn = myPawn;
}

DoAttackState::~DoAttackState()
{
}

void DoAttackState::Run()
{
	MyPawn->SpawnAttackProjectile();
	MyPawn->WaitAttackDelay();
	attackStateMachine->ChangeState(attackStateMachine->waitAttackState);
}
