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
	
	if (stateMachine->playerController)
	{
		myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());

		///Set Attack FrontDelay///
		if (myCharacter)
		{
			stateMachine->frontDelay = myCharacter->frontDelay;
		}
	}
}

void AttackState::Update(float DeltaTime)
{
	if (stateMachine->frontDelay <= 0 && myCharacter)
	{
		//Attack
		myCharacter->Attack(stateMachine->target);

		stateMachine->attackDelay = 1 / myCharacter->GetAttackSpeed();

		if (stateMachine->target != nullptr)
		{
			if (Cast<IDamagable>(stateMachine->target)->GetHP() <= 0)
			{
				stateMachine->target = nullptr;
				stateMachine->ChangeState(stateMachine->Idle);
				return;
			}
		}

		stateMachine->ChangeState(stateMachine->MoveToAttack);
	}
}

void AttackState::OnRightClick()
{
	stateMachine->HasAttackEnemy();
}

void AttackState::OnRightRelease()
{
}

void AttackState::OnLeftClick()
{
	if (stateMachine->bSkillReady && stateMachine->currentSkill != E_SkillState::Skill_Null && myCharacter->skills[stateMachine->currentSkill]->CanUse())
	{
		stateMachine->ChangeState(stateMachine->UseSkill);
	}
}

void AttackState::OnKeyboard(E_SkillState SkillState)
{
	stateMachine->ChangeCurrentSkill(SkillState);
}

void AttackState::OnEnd()
{
	
}