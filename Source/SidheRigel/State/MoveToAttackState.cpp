// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveToAttackState.h"

#include "SidheRigel/SidheRigelPlayerController.h"
#include "SidheRigel/SidheRigelCharacter.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"

MoveToAttackState::MoveToAttackState(StateMachine* StateMachine) : State(StateMachine)
{
}

MoveToAttackState::~MoveToAttackState()
{
}

void MoveToAttackState::OnBegin()
{
	UE_LOG(LogTemp, Warning, TEXT("MOVETOATTACK BEGIN"));

	if (stateMachine && stateMachine->playerController)
	{
		myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());
	}
}

void MoveToAttackState::Update(float DeltaTime)
{
	if (myCharacter && stateMachine)
	{
		//Can Attack Target
		if ((myCharacter->GetRange() >= myCharacter->GetDistanceTo(stateMachine->target)))
		{
			stateMachine->playerController->StopMovement();
			stateMachine->ChangeState(stateMachine->AttackWait);
		}
		else
		{
			if (stateMachine->target)
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(stateMachine->playerController, stateMachine->target->GetActorLocation());
			}
		}

		if (stateMachine->bAttackWithSkillReady)
		{
			//Show Skill Range
		}
	}
}

void MoveToAttackState::OnRightClick()
{
	if (stateMachine)
	{
		stateMachine->bAttackWithSkillReady = false;
		stateMachine->currentSkill = E_SkillState::Null;

		stateMachine->Idle->OnRightClick();
	}
}

void MoveToAttackState::OnRightRelease()
{
}

void MoveToAttackState::OnLeftClick()
{
	if (stateMachine->bAttackWithSkillReady)
	{
		stateMachine->bAttackWithSkillReady = false;
		stateMachine->ChangeState(stateMachine->UseSkill);
	}
}

void MoveToAttackState::OnKeyboard(E_SkillState SkillState)
{
	//Check Cooldown
	if (myCharacter->skills[SkillState]->GetCooldown() <= 0)
	{
		stateMachine->currentSkill = SkillState;

		//Check Instant cast
		if (myCharacter->skills[SkillState]->IsInstantCast())
		{
			stateMachine->ChangeState(stateMachine->UseSkill);
			return;
		}

		stateMachine->bAttackWithSkillReady = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SKILL HAS COOLTIME"));
	}
}

void MoveToAttackState::OnEnd()
{
}
