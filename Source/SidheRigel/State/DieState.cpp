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
	

	if (!myCharacter)
	{
		myCharacter = Cast<ASidheRigelCharacter>(controller->GetPawn());
	}

	myCharacter->GetMesh()->SetVisibility(false);
	myCharacter->TurnOffStatUI();
	
	myCharacter->SetDeathUI(myCharacter->DieTime);

	/*myCharacter->GetMesh()->SetCollisionProfileName(FName("NoCollision"));
	myCharacter->GetCapsuleComponent()->SetCollisionProfileName(FName("NoCollision"));*/
}

void UDieState::Update(float DeltaTime)
{
	if (myCharacter->DieTime <= 0)
	{
		myCharacter->isDie = false;
		
		myCharacter->GetMesh()->SetVisibility(true);
		myCharacter->SetActorLocation(myCharacter->spawnLocation);
		myCharacter->SetCurrentHP(myCharacter->GetMaxHP());
		myCharacter->TurnOnStatUI();
		myCharacter->ClearDeathUI();

		controller->ChangeState(controller->Idle);
		/*myCharacter->GetMesh()->SetCollisionProfileName(FName("BlockAll"));
		myCharacter->GetCapsuleComponent()->SetCollisionProfileName(FName("BlockAll"));*/
	}
	else
	{
		myCharacter->SetDeathUI(myCharacter->DieTime);
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
