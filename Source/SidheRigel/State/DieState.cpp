// Fill out your copyright notice in the Description page of Project Settings.


#include "DieState.h"

#include "StateMachine.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/SidheRigelPlayerController.h"

UDieState::UDieState()
{
}

UDieState::~UDieState()
{
}

void UDieState::OnBegin()
{
	if (!myCharacter)
	{
		myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());
	}
}

void UDieState::Update(float DeltaTime)
{
	if (stateMachine->DieTime <= 0)
	{
		myCharacter->isDie = false;
		stateMachine->ChangeState(stateMachine->Idle);
	}
}

void UDieState::OnRightClick()
{
}

void UDieState::OnRightRelease()
{
}

void UDieState::OnLeftClick()
{
}

void UDieState::OnKeyboard(E_SkillState SkillState)
{
}

void UDieState::OnEnd()
{
}
