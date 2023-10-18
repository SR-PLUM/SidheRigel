// Fill out your copyright notice in the Description page of Project Settings.


#include "DieState.h"

#include "StateMachine.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/SidheRigelPlayerController.h"

DieState::DieState(StateMachine* StateMachine) : State(StateMachine)
{
}

DieState::~DieState()
{
}

void DieState::OnBegin()
{
	if (!myCharacter)
	{
		myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());
	}
}

void DieState::Update(float DeltaTime)
{
	if (stateMachine->DieTime <= 0)
	{
		myCharacter->isDie = false;
		stateMachine->ChangeState(stateMachine->Idle);
	}
}

void DieState::OnRightClick()
{
}

void DieState::OnRightRelease()
{
}

void DieState::OnLeftClick()
{
}

void DieState::OnKeyboard(E_SkillState SkillState)
{
}

void DieState::OnEnd()
{
}
