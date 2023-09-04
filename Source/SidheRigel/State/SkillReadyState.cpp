// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillReadyState.h"

#include "StateMachine.h"
#include "SidheRigel/SidheRigelCharacter.h"

SkillReadyState::SkillReadyState(StateMachine* StateMachine) : State(StateMachine)
{
}

SkillReadyState::~SkillReadyState()
{
}

void SkillReadyState::OnBegin()
{
	UE_LOG(LogTemp, Warning, TEXT("SKILLREADY BEGIN"));

	if (stateMachine && stateMachine->playerController)
	{
		myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());
	}
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
	if (myCharacter->GetCooldown(SkillState) <= 0)
	{
		stateMachine->currentSkill = SkillState;

		stateMachine->ChangeState(stateMachine->SkillReady);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SKILL HAS COOLTIME"));
		stateMachine->currentSkill = E_SkillState::Null;
	}
}

void SkillReadyState::OnEnd()
{
}
