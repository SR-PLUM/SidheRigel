// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackState.h"

#include "StateMachine.h"
#include "SidheRigel/SidheRigelCharacter.h"

AttackState::AttackState(StateMachine* StateMachine) : State(StateMachine)
{
}

AttackState::~AttackState()
{
}

void AttackState::OnBegin()
{
	UE_LOG(LogTemp, Warning, TEXT("ATTACK BEGIN"));
		 ///set frontDelay///
	if (stateMachine->playerController)
	{
		myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());

		/*if (myCharacter)
		{
			stateMachine->frontDelay = myCharacter->frontDelay;
		}*/
	}
}

void AttackState::Update(float DeltaTime)
{
	if (stateMachine && stateMachine->frontDelay <= 0)
	{
		//Attack to Target
		//myCharacter->Attack(target);
		stateMachine->ChangeState(stateMachine->MoveToAttack);
	}
}

void AttackState::OnRightClick()
{
	stateMachine->bAttackWithSkillReady = false;
	stateMachine->currentSkill = E_SkillState::Null;

	stateMachine->Idle->OnRightClick();
}

void AttackState::OnRightRelease()
{
}

void AttackState::OnLeftClick()
{
	if (stateMachine->bAttackWithSkillReady)
	{
		stateMachine->bAttackWithSkillReady = false;
		stateMachine->ChangeState(stateMachine->UseSkill);
	}
}

void AttackState::OnKeyboard(E_SkillState SkillState)
{
	//Check Cooldown
	if (true)
	{
		stateMachine->bAttackWithSkillReady = true;
		stateMachine->currentSkill = SkillState;
	}
}

void AttackState::OnEnd()
{
	if (stateMachine && myCharacter)
	{
		stateMachine->attackDelay = 1 / myCharacter->GetAttackSpeed();
	}
}