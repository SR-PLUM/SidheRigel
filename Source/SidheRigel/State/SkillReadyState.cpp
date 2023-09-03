// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillReadyState.h"

#include "StateMachine.h"

SkillReadyState::SkillReadyState(StateMachine* StateMachine) : State(StateMachine)
{
}

SkillReadyState::~SkillReadyState()
{
}

void SkillReadyState::OnBegin()
{
	UE_LOG(LogTemp, Warning, TEXT("SKILLREADY BEGIN"));
}

void SkillReadyState::Update(float DeltaTime)
{
	//Show Skill Range
}

void SkillReadyState::OnRightClick()
{
	stateMachine->currentSkill = E_SkillState::Null;

	stateMachine->Idle->OnRightClick();
}

void SkillReadyState::OnRightRelease()
{
}

void SkillReadyState::OnLeftClick()
{
	stateMachine->ChangeState(stateMachine->UseSkill);
}

void SkillReadyState::OnKeyboard(E_SkillState SkillState)
{
	//Check Cooldown
	if (true)
	{
		stateMachine->currentSkill = SkillState;

		stateMachine->ChangeState(stateMachine->SkillReady);
	}
	else
	{
		stateMachine->currentSkill = E_SkillState::Null;
	}
}

void SkillReadyState::OnEnd()
{
}
