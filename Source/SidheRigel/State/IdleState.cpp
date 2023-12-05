// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleState.h"

#include "StateMachine.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/SidheRigelPlayerController.h"
#include "SidheRigel/Interface/Damagable.h"

UIdleState::UIdleState()
{
}

UIdleState::~UIdleState()
{
}

void UIdleState::OnBegin()
{
	if (stateMachine->playerController)
	{
		stateMachine->playerController->StopMovement();

		myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());
	}
}

void UIdleState::Update(float DeltaTime)
{
}

void UIdleState::OnRightClick()
{
	stateMachine->HasAttackEnemy();
}

void UIdleState::OnRightRelease()
{
}

void UIdleState::OnLeftClick()
{
	if (stateMachine->bSkillReady && stateMachine->currentSkill != E_SkillState::Skill_Null && myCharacter->skills[stateMachine->currentSkill]->CanUse())
	{
		stateMachine->ChangeState(stateMachine->UseSkill);
	}
}

void UIdleState::OnKeyboard(E_SkillState SkillState)
{
	stateMachine->ChangeCurrentSkill(SkillState);
}

void UIdleState::OnEnd()
{
}
