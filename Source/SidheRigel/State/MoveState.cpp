// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveState.h"

#include "StateMachine.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/SidheRigelPlayerController.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"

MoveState::MoveState(StateMachine* StateMachine) : State(StateMachine)
{
}

MoveState::~MoveState()
{
}

void MoveState::OnBegin()
{
	if (stateMachine->playerController)
	{
		myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());

		//input is being pressed
		bInputPressed = true;
		//previous move will stop
		stateMachine->playerController->StopMovement();

		stateMachine->target = nullptr;
	}
}

void MoveState::Update(float DeltaTime)
{
	if (stateMachine->playerController && myCharacter)
	{
		if (((myCharacter->GetActorLocation() == stateMachine->location) || myCharacter->GetVelocity().Size() < 0.4f) && bInputPressed == false)
		{
			stateMachine->ChangeState(stateMachine->Idle);
		}
		else
		{
			if (bInputPressed)	//LongClick
			{
				// Refresh the Location
				FHitResult Hit;
				stateMachine->playerController->GetHitResultUnderCursor(ECC_Visibility, true, Hit);
				stateMachine->location = Hit.Location;

				FVector WorldDirection = (stateMachine->location - myCharacter->GetActorLocation()).GetSafeNormal();
				
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(stateMachine->playerController, stateMachine->location);
			}
		}
	}
}

void MoveState::OnRightClick()
{
	stateMachine->HasAttackEnemy();
}

void MoveState::OnRightRelease()
{
	bInputPressed = false;

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(stateMachine->playerController, stateMachine->location);
}

void MoveState::OnLeftClick()
{
	if (stateMachine->bSkillReady && stateMachine->currentSkill != E_SkillState::Skill_Null && myCharacter->skills[stateMachine->currentSkill]->CanUse())
	{
		stateMachine->ChangeState(stateMachine->UseSkill);
	}
}

void MoveState::OnKeyboard(E_SkillState SkillState)
{
	stateMachine->ChangeCurrentSkill(SkillState);
}

void MoveState::OnEnd()
{
}
