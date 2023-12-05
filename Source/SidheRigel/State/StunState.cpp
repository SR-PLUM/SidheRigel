// Fill out your copyright notice in the Description page of Project Settings.


#include "StunState.h"

#include "SidheRigel/Enum/E_SkillState.h"
#include "SidheRigel/State/StateMachine.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/SidheRigelPlayerController.h"

UStunState::UStunState()
{
}

UStunState::~UStunState()
{
}

void UStunState::OnBegin()
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

void UStunState::Update(float DeltaTime)
{
	if (stateMachine->stunTime <= 0)
	{
		stateMachine->ChangePreviousState();
	}
}

void UStunState::OnRightClick()
{
}

void UStunState::OnRightRelease()
{
}

void UStunState::OnLeftClick()
{
}

void UStunState::OnKeyboard(E_SkillState SkillState)
{
}

void UStunState::OnEnd()
{
	myCharacter->RemoveStunParticle();
}
