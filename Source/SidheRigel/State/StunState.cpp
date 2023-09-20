// Fill out your copyright notice in the Description page of Project Settings.


#include "StunState.h"

#include "SidheRigel/Enum/E_SkillState.h"
#include "SidheRigel/State/StateMachine.h"

StunState::StunState(StateMachine* StateMachine) : State(StateMachine)
{
}

StunState::~StunState()
{
}

void StunState::OnBegin()
{
	UE_LOG(LogTemp, Warning, TEXT("Stun"));
}

void StunState::Update(float DeltaTime)
{
	if (stateMachine->stunTime <= 0)
	{
		stateMachine->ChangeState(stateMachine->Idle);
	}
}

void StunState::OnRightClick()
{
}

void StunState::OnRightRelease()
{
}

void StunState::OnLeftClick()
{
}

void StunState::OnKeyboard(E_SkillState SkillState)
{
}

void StunState::OnEnd()
{
}
