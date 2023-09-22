// Fill out your copyright notice in the Description page of Project Settings.


#include "StopState.h"

#include "SidheRigel/Enum/E_SkillState.h"
#include "StateMachine.h"
#include "SidheRigel/SidheRigelCharacter.h"

StopState::StopState(StateMachine* StateMachine) : State(StateMachine)
{
}

StopState::~StopState()
{
}

void StopState::OnBegin()
{
	if (stateMachine->playerController)
	{
		myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());
	}
}

void StopState::Update(float DeltaTime)
{
	//Can Attack
	if (stateMachine->target)
	{
		if (myCharacter->GetRange() >= myCharacter->GetDistanceTo(stateMachine->target))
		{
			if (stateMachine->attackDelay <= 0)
			{
				myCharacter->Attack(stateMachine->target);
				stateMachine->attackDelay = 1 / myCharacter->GetAttackSpeed();
			}
		}
	}

	if (stateMachine->stopTime <= 0)
	{
		stateMachine->ChangePreviousState();
	}
}

void StopState::OnRightClick()
{
	//Set Target
	if (stateMachine->playerController)
	{
		AActor* HitActor = stateMachine->playerController->GetHitResult().GetActor();
		if (HitActor)
		{
			IDamagable* DamagableActor = Cast<IDamagable>(HitActor);
			if (DamagableActor)
			{
				stateMachine->target = HitActor;
			}
		}
	}
}

void StopState::OnRightRelease()
{
}

void StopState::OnLeftClick()
{
	if (stateMachine->bSkillReady && stateMachine->currentSkill != E_SkillState::Null)
	{
		stateMachine->ChangeState(stateMachine->UseSkill);
	}
}

void StopState::OnKeyboard(E_SkillState SkillState)
{
	//TODO : CANT USE MOVING SKILL
	stateMachine->ChangeCurrentSkill(SkillState);
}

void StopState::OnEnd()
{
}
