// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackWaitState.h"

#include "StateMachine.h"
#include "SidheRigel/SidheRigelCharacter.h"

AttackWaitState::AttackWaitState(StateMachine* StateMachine) : State(StateMachine)
{
}

AttackWaitState::~AttackWaitState()
{
}

void AttackWaitState::OnBegin()
{
	UE_LOG(LogTemp, Warning, TEXT("ATTACKWAIT BEGIN"));
	if (stateMachine && stateMachine->playerController)
	{
		myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());
	}
}

void AttackWaitState::Update(float DeltaTime)
{
	if (stateMachine)
	{
		if (stateMachine->attackDelay <= 0)
		{
			stateMachine->ChangeState(stateMachine->Attack);
		}
		if (myCharacter)
		{
			if (myCharacter->GetRange() < myCharacter->GetDistanceTo(stateMachine->target))
			{
				stateMachine->ChangeState(stateMachine->MoveToAttack);
			}
		}
	}
}

void AttackWaitState::OnRightClick()
{
	stateMachine->Idle->OnRightClick();
}

void AttackWaitState::OnRightRelease()
{
}

void AttackWaitState::OnLeftClick()
{
}

void AttackWaitState::OnKeyboard()
{
}

void AttackWaitState::OnEnd()
{
}
