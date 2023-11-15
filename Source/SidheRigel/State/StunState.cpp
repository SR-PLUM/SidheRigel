// Fill out your copyright notice in the Description page of Project Settings.


#include "StunState.h"

#include "SidheRigel/Enum/E_SkillState.h"
#include "SidheRigel/State/StateMachine.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/SidheRigelPlayerController.h"

StunState::StunState(StateMachine* StateMachine) : State(StateMachine)
{
}

StunState::~StunState()
{
}

void StunState::OnBegin()
{
	if (stateMachine->playerController)
	{
		myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());

		if (myCharacter)
		{
			myCharacter->SpawnStunParticle();
		}
	}
}

void StunState::Update(float DeltaTime)
{
	if (stateMachine->stunTime <= 0)
	{
		stateMachine->ChangePreviousState();
	}
}

void StunState::OnRightClick()
{
}

void StunState::OnRightRelease()
{
}

void StunState::OnLeftClick()
{
}

void StunState::OnKeyboard(E_SkillState SkillState)
{
}

void StunState::OnEnd()
{
	myCharacter->RemoveStunParticle();
}
