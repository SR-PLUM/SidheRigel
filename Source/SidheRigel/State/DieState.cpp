// Fill out your copyright notice in the Description page of Project Settings.


#include "DieState.h"

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
		myCharacter = Cast<ASidheRigelCharacter>(controller->GetPawn());
		controller->StopMovement();
	}
}

void UDieState::Update(float DeltaTime)
{
	if (myCharacter->DieTime <= 0)
	{
		myCharacter->isDie = false;
		myCharacter->SetActorLocation(myCharacter->spawnLocation);
		myCharacter->SetCurrentHP(myCharacter->GetMaxHP());

		controller->ChangeState(controller->Idle);
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
