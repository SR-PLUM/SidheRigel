// Fill out your copyright notice in the Description page of Project Settings.


#include "DieState.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/SidheRigelPlayerController.h"
#include "Components/CapsuleComponent.h"

UDieState::UDieState()
{
}

UDieState::~UDieState()
{
}

void UDieState::OnBegin()
{
	controller->StopMovement();
	//myCharacter->GetMesh()->SetCollisionProfileName(FName("NoCollision"));
	//myCharacter->GetCapsuleComponent()->SetCollisionProfileName(FName("NoCollision"));

	if (!myCharacter)
	{
		myCharacter = Cast<ASidheRigelCharacter>(controller->GetPawn());
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
		//myCharacter->GetMesh()->SetCollisionProfileName(FName("BlockAll"));
		//myCharacter->GetCapsuleComponent()->SetCollisionProfileName(FName("BlockAll"));
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
