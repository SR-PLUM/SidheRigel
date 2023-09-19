// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine.h"

#include "State.h"
#include "IdleState.h"
#include "MoveToAttackState.h"
#include "MoveState.h"
#include "AttackWaitState.h"
#include "AttackState.h"
#include "SkillReadyState.h"
#include "UseSkillState.h"
#include "StunState.h"
#include "SidheRigel/SidheRigelPlayerController.h"

StateMachine::StateMachine(ASidheRigelPlayerController* PlayerController)
{
	Idle = new IdleState(this);
	MoveToAttack = new MoveToAttackState(this);
	Move = new MoveState(this);
	AttackWait = new AttackWaitState(this);
	Attack = new AttackState(this);
	SkillReady = new SkillReadyState(this);
	UseSkill = new UseSkillState(this);

	Stun = new StunState(this);

	currentState = Idle;

	playerController = PlayerController;
}

StateMachine::~StateMachine()
{
}

void StateMachine::ChangeState(State* NextState)
{
	currentState->OnEnd();
	NextState->OnBegin();
	currentState = NextState;
}

void StateMachine::Update(float DeltaTime)
{
	if (attackDelay > 0)
	{
		attackDelay -= DeltaTime;
	}
	if (frontDelay > 0)
	{
		frontDelay -= DeltaTime;
	}
	if (stunTime > 0)
	{
		stunTime -= DeltaTime;
	}
	if (stopTime > 0)
	{
		stopTime -= DeltaTime;
	}

	currentState->Update(DeltaTime);
}

void StateMachine::OnRightClick()
{
	currentState->OnRightClick();
}

void StateMachine::OnRightRelease()
{
	currentState->OnRightRelease();
}

void StateMachine::OnLeftClick()
{
	currentState->OnLeftClick();
}

void StateMachine::OnKeyboard(E_SkillState SkillState)
{
	currentState->OnKeyboard(SkillState);
}

void StateMachine::OnStun(float _stunTime)
{
	stunTime = _stunTime;
	ChangeState(Stun);
}
