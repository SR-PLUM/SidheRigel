// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveToAttackState.h"

MoveToAttackState::MoveToAttackState(AttackStateMachine* attackStateMachine, ASidheRigelCharacter* myPawn)
{
	this->attackStateMachine = attackStateMachine;
	MyPawn = myPawn;
}

MoveToAttackState::~MoveToAttackState()
{
}

void MoveToAttackState::Run()
{
	if (MyPawn)
	{
		if (MyPawn->target)
		{
			if (MyPawn->GetDistanceTo(MyPawn->target) <= MyPawn->GetRange())	//Ÿ���� ��Ÿ� �� ������ ����
			{
				attackStateMachine->ChangeState(attackStateMachine->doAttackState);
			}
			else
			{
				FVector WorldDirection = (MyPawn->target->GetActorLocation() - MyPawn->GetActorLocation()).GetSafeNormal();
				MyPawn->AddMovementInput(WorldDirection, 1.f, false);
			}
		}
	}
}
