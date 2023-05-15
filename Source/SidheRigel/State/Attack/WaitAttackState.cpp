// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitAttackState.h"

WaitAttackState::WaitAttackState(AttackStateMachine* attackStateMachine, ASidheRigelCharacter* myPawn)
{
	this->attackStateMachine = attackStateMachine;
	MyPawn = myPawn;
}

WaitAttackState::~WaitAttackState()
{
}

void WaitAttackState::Run()
{
	//한번만 실행되게 교체
	//FTimerHandle AttackDelayTimer;

	//MyPawn->GetWorldTimerManager().SetTimer(AttackDelayTimer, MyPawn, &ASidheRigelCharacter::WaitAttackDelay, 1 / MyPawn->GetAttackSpeed(), false);
}
