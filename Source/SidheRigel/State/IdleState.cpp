// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleState.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/SidheRigelPlayerController.h"
#include "SidheRigel/Interface/Damagable.h"

UIdleState::UIdleState()
{
}

UIdleState::~UIdleState()
{
}

void UIdleState::OnBegin()
{
	if (controller)
	{
		controller->StopMovement();

		myCharacter = Cast<ASidheRigelCharacter>(controller->GetPawn());
	}
}

void UIdleState::Update(float DeltaTime)
{
}

void UIdleState::OnRightClick()
{
	controller->HasAttackEnemy();
}

void UIdleState::OnRightRelease()
{
}

void UIdleState::OnLeftClick()
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
}

void UIdleState::OnKeyboard(E_SkillState SkillState)
{
	controller->ChangeCurrentSkill(SkillState);
}

void UIdleState::OnEnd()
{
}
