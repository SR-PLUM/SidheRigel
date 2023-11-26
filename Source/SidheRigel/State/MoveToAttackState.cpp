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
	if (stateMachine->playerController)
	{
		myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());
	}
}

void MoveToAttackState::Update(float DeltaTime)
{
	if (myCharacter)
	{
		//Can Attack Target
		if ((myCharacter->GetRange() >= myCharacter->GetDistanceTo(stateMachine->target)))
		{
			stateMachine->playerController->StopMovement();
			stateMachine->ChangeState(stateMachine->AttackWait);
		}
		else
		{
			//Move
			if (stateMachine->target)
			{
				myCharacter->Server_MoveToPoint(stateMachine->location);
				//UAIBlueprintHelperLibrary::SimpleMoveToLocation(stateMachine->playerController, stateMachine->target->GetActorLocation());
			}
		}
	}
}

void MoveToAttackState::OnRightClick()
{
	stateMachine->HasAttackEnemy();
}

void MoveToAttackState::OnRightRelease()
{
}

void MoveToAttackState::OnLeftClick()
{
	if (stateMachine->bSkillReady && stateMachine->currentSkill != E_SkillState::Skill_Null && myCharacter->skills[stateMachine->currentSkill]->CanUse())
	{
		stateMachine->ChangeState(stateMachine->UseSkill);
	}
}

void MoveToAttackState::OnKeyboard(E_SkillState SkillState)
{
	stateMachine->ChangeCurrentSkill(SkillState);
}

void MoveToAttackState::OnEnd()
{
}
