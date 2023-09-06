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

		if (stateMachine->bAttackWithSkillReady)
		{
			//Show Skill Range
		}
	}
}

void AttackWaitState::OnRightClick()
{
	stateMachine->bAttackWithSkillReady = false;
	stateMachine->currentSkill = E_SkillState::Null;

	stateMachine->Idle->OnRightClick();
}

void AttackWaitState::OnRightRelease()
{
}

void AttackWaitState::OnLeftClick()
{
	if (stateMachine->bAttackWithSkillReady)
	{
		stateMachine->bAttackWithSkillReady = false;
		stateMachine->ChangeState(stateMachine->UseSkill);
	}
}

void AttackWaitState::OnKeyboard(E_SkillState SkillState)
{
	//Check Cooldown
	if (myCharacter->GetCooldown(SkillState) <= 0)
	{
		stateMachine->currentSkill = SkillState;

		//Check Instant cast
		if (myCharacter->IsInstantCast(stateMachine->currentSkill))
		{
			stateMachine->ChangeState(stateMachine->UseSkill);
			return;
		}

		stateMachine->bAttackWithSkillReady = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SKILL HAS COOLTIME"));
	}
}

void AttackWaitState::OnEnd()
{
}
