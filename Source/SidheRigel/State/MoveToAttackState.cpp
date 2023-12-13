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
	if (controller)
	{
		if (!myCharacter)
		{
			myCharacter = Cast<ASidheRigelCharacter>(controller->GetPawn());
		}
	}
}

void UMoveToAttackState::Update(float DeltaTime)
{
	if (myCharacter)
	{
		//Can Attack Target
		if ((myCharacter->GetRange() >= myCharacter->GetDistanceTo(controller->target)))
		{
			controller->StopMovement();
			controller->ChangeState(controller->AttackWait);
		}
		else
		{
			//Move
			if (controller->target)
			{
				myCharacter->Server_MoveToPoint(controller->target->GetActorLocation());
				//UAIBlueprintHelperLibrary::SimpleMoveToLocation(stateMachine->playerController, stateMachine->target->GetActorLocation());
			}
		}
	}
}

void UMoveToAttackState::OnRightClick()
{
	controller->HasAttackEnemy();
}

void UMoveToAttackState::OnRightRelease()
{
}

void UMoveToAttackState::OnLeftClick()
{
	if (controller->bSkillReady)
	{
		if (controller->currentSkill != E_SkillState::Skill_Null)
		{
			if (myCharacter->skills[controller->currentSkill]->CanUse())
			{
				controller->ChangeState(controller->UseSkill);
			}
		}
	}
	else if (controller->bAttackReady)
	{
		auto tempTarget = controller->GetCloseActorToMouse();

		myCharacter->skillRange->SetVisibility(false);

		if (tempTarget != nullptr)
		{
			controller->bAttackReady = false;

			controller->target = tempTarget;
			controller->ChangeState(controller->MoveToAttack);
		}
		else if (tempTarget == nullptr)
		{
			controller->location = controller->GetHitResult().Location;
			controller->ChangeState(controller->Move);
		}
	}
}

void UMoveToAttackState::OnKeyboard(E_SkillState SkillState)
{
	controller->ChangeCurrentSkill(SkillState);
}

void UMoveToAttackState::OnEnd()
{
}
