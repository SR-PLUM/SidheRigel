// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackState.h"

#include "StateMachine.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/SidheRigelPlayerController.h"

UAttackState::UAttackState()
{
}

UAttackState::~UAttackState()
{
}

void UAttackState::OnBegin()
{
	UE_LOG(LogTemp,Warning,TEXT("AttackState :: Begin"))
	if (stateMachine->playerController)
	{
		myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());

		///Set Attack FrontDelay///
		if (myCharacter)
		{
			stateMachine->frontDelay = myCharacter->frontDelay;
		}

		//Turn to Target
		if (stateMachine->target)
		{
			FVector ForwardDirection = ((stateMachine->target->GetActorLocation() - myCharacter->GetActorLocation()) * FVector(1, 1, 0)).GetSafeNormal();
			myCharacter->SetActorRotation(ForwardDirection.Rotation());
		}
	}
}

void UAttackState::Update(float DeltaTime)
{
	if (stateMachine->frontDelay <= 0 && myCharacter)
	{
		//Attack
		myCharacter->Attack(stateMachine->target);

		stateMachine->attackDelay = 1 / myCharacter->GetAttackSpeed();

		if (stateMachine->target != nullptr)
		{
			if (Cast<IDamagable>(stateMachine->target)->GetHP() <= 0)
			{
				stateMachine->target = nullptr;
				stateMachine->ChangeState(stateMachine->Idle);
				return;
			}
		}

		stateMachine->ChangeState(stateMachine->MoveToAttack);
	}
}

void UAttackState::OnRightClick()
{
	stateMachine->HasAttackEnemy();
}

void UAttackState::OnRightRelease()
{
}

void UAttackState::OnLeftClick()
{
	if (stateMachine->bSkillReady && stateMachine->currentSkill != E_SkillState::Skill_Null && myCharacter->skills[stateMachine->currentSkill]->CanUse())
	{
		stateMachine->ChangeState(stateMachine->UseSkill);
	}
}

void UAttackState::OnKeyboard(E_SkillState SkillState)
{
	stateMachine->ChangeCurrentSkill(SkillState);
}

void UAttackState::OnEnd()
{
	
}