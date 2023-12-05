// Fill out your copyright notice in the Description page of Project Settings.


#include "State.h"

#include "StateMachine.h"
#include "SidheRigel/SidheRigelPlayerController.h"

UState::UState()
{
}

void UState::InitStateMachine(UStateMachine* StateMachine)
{
	stateMachine = StateMachine;
}

UState::~UState()
{
}

void UState::OnBegin()
{
}

void UState::Update(float DeltaTime)
{
}

void UState::OnRightClick()
{
}

void UState::OnRightRelease()
{
}

void UState::OnLeftClick()
{
}

void UState::OnKeyboard(E_SkillState SkillState)
{
}

void UState::OnEnd()
{
}
