// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackWaitState.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/SidheRigelPlayerController.h"

UAttackWaitState::UAttackWaitState()
{
}

UAttackWaitState::~UAttackWaitState()
{
}

void UAttackWaitState::OnBegin()
{
	if (controller)
	{
		myCharacter = Cast<ASidheRigelCharacter>(controller->GetPawn());
	}
}

void UAttackWaitState::Update(float DeltaTime)
{
	//Can Attack
	if (controller->attackDelay <= 0 && myCharacter->stunTime <= 0)
	{
		controller->ChangeState(controller->Attack);
	}

	//Have to Move
	if (myCharacter)
	{
		if (myCharacter->GetRange() < myCharacter->GetDistanceTo(controller->target))
		{
			controller->ChangeState(controller->MoveToAttack);
		}
	}
}

void UAttackWaitState::OnRightClick()
{
	controller->HasAttackEnemy();
}

void UAttackWaitState::OnRightRelease()
{
}

void UAttackWaitState::OnLeftClick()
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

void UAttackWaitState::OnKeyboard(E_SkillState SkillState)
{
	controller->ChangeCurrentSkill(SkillState);
}

void UAttackWaitState::OnEnd()
{
}
