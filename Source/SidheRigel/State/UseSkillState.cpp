// Fill out your copyright notice in the Description page of Project Settings.


#include "UseSkillState.h"

#include "StateMachine.h"
#include "SidheRigel/SidheRigelCharacter.h"

UseSkillState::UseSkillState(StateMachine* StateMachine) : State(StateMachine)
{
}

UseSkillState::~UseSkillState()
{
}

void UseSkillState::OnBegin()
{
	UE_LOG(LogTemp, Warning, TEXT("USE SKILL BEGIN"));
	stateMachine->playerController->StopMovement();

	//TODO : Change Function
	float GetDelay = 1;
	stateMachine->SkillDelay = GetDelay;

	//USE SKILL
	ASidheRigelCharacter* myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());
	if (myCharacter)
	{
		//myCharacter->
	}
}

void UseSkillState::Update(float DeltaTime)
{
	if (stateMachine->SkillDelay > 0)
	{
		stateMachine->SkillDelay -= DeltaTime;
	}
	else
	{
		stateMachine->currentSkill = E_SkillState::Null;
		stateMachine->ChangeState(stateMachine->Idle);
	}
}

void UseSkillState::OnRightClick()
{
}

void UseSkillState::OnRightRelease()
{
}

void UseSkillState::OnLeftClick()
{
}

void UseSkillState::OnKeyboard(E_SkillState SkillState)
{
}

void UseSkillState::OnEnd()
{
}
