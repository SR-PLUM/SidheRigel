// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveToAttackState.h"

#include "SidheRigel/SidheRigelPlayerController.h"
#include "SidheRigel/SidheRigelCharacter.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"

UMoveToAttackState::UMoveToAttackState()
{
}

UMoveToAttackState::~UMoveToAttackState()
{
}

void UMoveToAttackState::OnBegin()
{
	if (stateMachine->playerController)
	{
		myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());
	}
}

void UMoveToAttackState::Update(float DeltaTime)
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
				myCharacter->Server_MoveToPoint(stateMachine->target->GetActorLocation());
				//UAIBlueprintHelperLibrary::SimpleMoveToLocation(stateMachine->playerController, stateMachine->target->GetActorLocation());
			}
		}
	}
}

void UMoveToAttackState::OnRightClick()
{
	stateMachine->HasAttackEnemy();
}

void UMoveToAttackState::OnRightRelease()
{
}

void UMoveToAttackState::OnLeftClick()
{
	if (stateMachine->bSkillReady && stateMachine->currentSkill != E_SkillState::Skill_Null && myCharacter->skills[stateMachine->currentSkill]->CanUse())
	{
		stateMachine->ChangeState(stateMachine->UseSkill);
	}
}

void UMoveToAttackState::OnKeyboard(E_SkillState SkillState)
{
	stateMachine->ChangeCurrentSkill(SkillState);
}

void UMoveToAttackState::OnEnd()
{
}
