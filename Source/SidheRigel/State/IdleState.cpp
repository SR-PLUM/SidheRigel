// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleState.h"

#include "StateMachine.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Interface/Damagable.h"

IdleState::IdleState(StateMachine* StateMachine) : State(StateMachine)
{
	
}

IdleState::~IdleState()
{
}

void IdleState::OnBegin()
{
	UE_LOG(LogTemp, Warning, TEXT("IDLE BEGIN"));

	if (stateMachine->playerController)
	{
		stateMachine->playerController->StopMovement();

		myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());
	}
}

void IdleState::Update(float DeltaTime)
{
}

void IdleState::OnRightClick()
{
	stateMachine->HasAttackEnemy();
}

void IdleState::OnRightRelease()
{
}

void IdleState::OnLeftClick()
{
	if (stateMachine->bSkillReady)
	{
		stateMachine->ChangeState(stateMachine->UseSkill);
	}
}

void IdleState::OnKeyboard(E_SkillState SkillState)
{
	stateMachine->ChangeCurrentSkill(SkillState);
}

void IdleState::OnEnd()
{
}
