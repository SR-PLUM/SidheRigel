// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackWaitState.h"

#include "StateMachine.h"
#include "SidheRigel/SidheRigelCharacter.h"

UAttackWaitState::UAttackWaitState()
{
}

UAttackWaitState::~UAttackWaitState()
{
}

void UAttackWaitState::OnBegin()
{
	if (stateMachine->playerController)
	{
		myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());
	}
}

void UAttackWaitState::Update(float DeltaTime)
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

void UAttackWaitState::OnRightClick()
{
	stateMachine->HasAttackEnemy();
}

void UAttackWaitState::OnRightRelease()
{
}

void UAttackWaitState::OnLeftClick()
{
	if (stateMachine->bSkillReady && stateMachine->currentSkill != E_SkillState::Skill_Null && myCharacter->skills[stateMachine->currentSkill]->CanUse())
	{
		stateMachine->ChangeState(stateMachine->UseSkill);
	}
}

void UAttackWaitState::OnKeyboard(E_SkillState SkillState)
{
	stateMachine->ChangeCurrentSkill(SkillState);
}

void UAttackWaitState::OnEnd()
{
}
