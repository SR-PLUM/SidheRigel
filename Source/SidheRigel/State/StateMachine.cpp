// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine.h"

#include "State.h"
#include "IdleState.h"
#include "MoveToAttackState.h"
#include "MoveState.h"
#include "AttackWaitState.h"
#include "AttackState.h"
#include "UseSkillState.h"
#include "StunState.h"
#include "DieState.h"
#include "SidheRigel/SidheRigelPlayerController.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Interface/Damagable.h"

#include "GameFramework/CharacterMovementComponent.h"

StateMachine::StateMachine(ASidheRigelPlayerController* PlayerController)
{
	Idle = new IdleState(this);
	MoveToAttack = new MoveToAttackState(this);
	Move = new MoveState(this);
	AttackWait = new AttackWaitState(this);
	Attack = new AttackState(this);
	UseSkill = new UseSkillState(this);

	Stun = new StunState(this);
	Die = new DieState(this);

	currentState = Idle;

	playerController = PlayerController;

	myCharacter = Cast<ASidheRigelCharacter>(playerController->GetPawn());
}

StateMachine::~StateMachine()
{
}

void StateMachine::ChangeState(State* NextState)
{
	previousState = currentState;
	currentState->OnEnd();
	NextState->OnBegin();
	currentState = NextState;
}

void StateMachine::ChangePreviousState()
{
	currentState->OnEnd();
	previousState->OnBegin();

	if (previousState == UseSkill)
	{
		previousState = Idle;
	}

	currentState = previousState;
}

State* StateMachine::GetCurrentState()
{
	return currentState;
}

//Timer
void StateMachine::Update(float DeltaTime)
{
	if (attackDelay > 0)
	{
		attackDelay -= DeltaTime;
	}
	if (frontDelay > 0)
	{
		frontDelay -= DeltaTime;
	}
	if (skillDelay > 0)
	{
		skillDelay -= DeltaTime;
	}
	if (stunTime > 0)
	{
		stunTime -= DeltaTime;
	}
	if (stopTime > 0)
	{
		stopTime -= DeltaTime;
		ChangeCharacterSpeed(0);
	}
	else
	{
		if (myCharacter)
		{
			ChangeCharacterSpeed(myCharacter->GetSpeed());
		}
		else
		{
			myCharacter = Cast<ASidheRigelCharacter>(playerController->GetPawn());
		}
	}
	if (silenceTime > 0)
	{
		silenceTime -= DeltaTime;
	}
	if (DieTime > 0)
	{
		DieTime -= DeltaTime;
	}

	currentState->Update(DeltaTime);
}

//Move Or Attack Or SkillCancel
void StateMachine::OnRightClick()
{
	//SkillCancel
	bSkillReady = false;
	currentSkill = E_SkillState::Skill_Null;
	myCharacter->skillRange->SetVisibility(false);

	currentState->OnRightClick();
}

//Move
void StateMachine::OnRightRelease()
{
	currentState->OnRightRelease();
}

//SkillUse
void StateMachine::OnLeftClick()
{
	currentState->OnLeftClick();
}

//SkillReady
void StateMachine::OnKeyboard(E_SkillState SkillState)
{
	if (silenceTime > 0)
	{
		return;
	}

	bSkillReady = true;
	currentSkill = SkillState;

	float currentSkillRange = myCharacter->skills[currentSkill]->GetRange();
	UE_LOG(LogTemp, Warning, TEXT("SkillRange : %f"), currentSkillRange);
	myCharacter->skillRange->SetRelativeScale3D(FVector(currentSkillRange / 100, currentSkillRange / 100, 1));

	myCharacter->skillRange->SetVisibility(true);

	currentState->OnKeyboard(SkillState);
}

void StateMachine::OnStun(float _stunTime)
{
	stunTime = _stunTime;
	ChangeState(Stun);
}

void StateMachine::OnStop(float _stopTime)
{
	stopTime = _stopTime;
}

void StateMachine::OnSilence(float _silenceTime)
{
	silenceTime = _silenceTime;
}

void StateMachine::HasAttackEnemy()
{
	//Move & Attack
	if (playerController)
	{
		AActor* HitActor = playerController->GetHitResult().GetActor();
		if (HitActor)
		{
			IDamagable* DamagableActor = Cast<IDamagable>(HitActor);
			if (DamagableActor)
			{
				ITeam* TeamActor = Cast<ITeam>(HitActor);
				if(TeamActor && TeamActor->GetTeam() != myCharacter->GetTeam())
				{
					target = HitActor;
					ChangeState(MoveToAttack);
				}
			}
			else
			{
				location = playerController->GetHitResult().Location;
				ChangeState(Move);
			}
		}
	}
}

void StateMachine::ChangeCurrentSkill(E_SkillState SkillState)
{
	//Check Cooldown
	if (myCharacter->skills[SkillState]->GetCooldown() <= 0)
	{
		currentSkill = SkillState;
		bSkillReady = true;

		//Check Instant cast
		if (myCharacter->skills[SkillState]->IsInstantCast() && myCharacter->skills[SkillState]->CanUse())
		{
			ChangeState(UseSkill);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SKILL HAS COOLTIME"));
		currentSkill = E_SkillState::Skill_Null;
		bSkillReady = false;
		myCharacter->skillRange->SetVisibility(false);
	}
}

void StateMachine::ChangeCharacterSpeed(float speed)
{
	myCharacter->GetCharacterMovement()->MaxWalkSpeed = speed;
}
