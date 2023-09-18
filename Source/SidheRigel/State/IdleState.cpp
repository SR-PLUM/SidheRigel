// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleState.h"

#include "StateMachine.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Interface/Damagable.h"

IdleState::IdleState(StateMachine* StateMachine) : State(StateMachine)
{
	
}

IdleState::~IdleState()
{
}

void IdleState::OnBegin()
{
	UE_LOG(LogTemp, Warning, TEXT("IDLE BEGIN"));
	if (stateMachine && stateMachine->playerController)
	{
		stateMachine->playerController->StopMovement();

		myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());
	}
}

void IdleState::Update(float DeltaTime)
{
}

void IdleState::OnRightClick()
{
	if (stateMachine && stateMachine->playerController)
	{
		AActor* HitActor = stateMachine->playerController->GetHitResult().GetActor();
		if (HitActor)
		{
			IDamagable* DamagableActor = Cast<IDamagable>(HitActor);
			if (DamagableActor)
			{
				if (stateMachine)
				{
					stateMachine->target = HitActor;
					stateMachine->ChangeState(stateMachine->MoveToAttack);
				}
			}
			else
			{
				if (stateMachine)
				{
					stateMachine->location = stateMachine->playerController->GetHitResult().Location;
					stateMachine->ChangeState(stateMachine->Move);
				}
			}
		}
	}
}

void IdleState::OnRightRelease()
{
}

void IdleState::OnLeftClick()
{
}

void IdleState::OnKeyboard(E_SkillState SkillState)
{
	//Check Cooldown
	if (myCharacter->skills[SkillState]->GetCooldown() <= 0)
	{
		stateMachine->currentSkill = SkillState;

		stateMachine->ChangeState(stateMachine->SkillReady);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SKILL HAS COOLTIME"));
	}
}

void IdleState::OnEnd()
{
}
