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

	if (stateMachine->playerController)
	{
		myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());
	}
}

void AttackWaitState::Update(float DeltaTime)
{
	//Can Attack
	if (stateMachine->attackDelay <= 0)
	{
		stateMachine->ChangeState(stateMachine->Attack);
	}

	//Have to Move
	if (myCharacter)
	{
		if (myCharacter->GetRange() < myCharacter->GetDistanceTo(stateMachine->target))
		{
			stateMachine->ChangeState(stateMachine->MoveToAttack);
		}
	}
}

void AttackWaitState::OnRightClick()
{
	stateMachine->HasAttackEnemy();
}

void AttackWaitState::OnRightRelease()
{
}

void AttackWaitState::OnLeftClick()
{
	if (stateMachine->bSkillReady && stateMachine->currentSkill != E_SkillState::Skill_Null && myCharacter->skills[stateMachine->currentSkill]->CanUse())
	{
		stateMachine->ChangeState(stateMachine->UseSkill);
	}
}

void AttackWaitState::OnKeyboard(E_SkillState SkillState)
{
	stateMachine->ChangeCurrentSkill(SkillState);
}

void AttackWaitState::OnEnd()
{
}
