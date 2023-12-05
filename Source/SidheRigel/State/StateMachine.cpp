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

UStateMachine::UStateMachine()
{
	
}

UStateMachine::~UStateMachine()
{
}

void UStateMachine::InitializeController(ASidheRigelPlayerController* Controller)
{
	Idle = NewObject<UIdleState>();
	Idle->InitStateMachine(this);
	MoveToAttack = NewObject<UMoveToAttackState>();
	MoveToAttack->InitStateMachine(this);
	Move = NewObject<UMoveState>();
	Move->InitStateMachine(this);
	AttackWait = NewObject<UAttackWaitState>();
	AttackWait->InitStateMachine(this);
	Attack = NewObject<UAttackState>();
	Attack->InitStateMachine(this);
	UseSkill = NewObject<UUseSkillState>();
	UseSkill->InitStateMachine(this);

	Stun = NewObject<UStunState>();
	Stun->InitStateMachine(this);
	Die = NewObject<UDieState>();
	Die->InitStateMachine(this);

	currentState = Idle;

	playerController = Controller;

	//myCharacter = Cast<ASidheRigelCharacter>(playerController->GetPawn());

	bIsinitialized = true;
}

void UStateMachine::ChangeState(UState* NextState)
{
	previousState = currentState;
	currentState->OnEnd();
	if (NextState)
	{
		NextState->OnBegin();
		currentState = NextState;
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("NextState Is Null"))
	}
}

void UStateMachine::ChangePreviousState()
{
	currentState->OnEnd();
	previousState->OnBegin();

	if (previousState == UseSkill)
	{
		previousState = Idle;
	}

	currentState = previousState;
}

UState* UStateMachine::GetCurrentState()
{
	return currentState;
}

//Timer
void UStateMachine::Update()
{
	if (!bIsinitialized)
		return;
	if (myCharacter)
	{
		float DeltaTime = 0.05f;
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
			if (stopTime <= 0)
			{
				myCharacter->RemoveStopParticle();
			}
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
			if (silenceTime <= 0)
			{
				myCharacter->RemoveSilenceParticle();
			}
		}
		if (DieTime > 0)
		{
			DieTime -= DeltaTime;
		}

		if (currentState)
		{
			currentState->Update(DeltaTime);
		}
	}
	else
	{
		myCharacter = Cast<ASidheRigelCharacter>(playerController->GetPawn());
	}
}

//Move Or Attack Or SkillCancel
void UStateMachine::OnRightClick()
{
	if (!bIsinitialized)
		return;
	//UE_LOG(LogTemp, Error, TEXT("OnRightClick"))
	//SkillCancel
	bSkillReady = false;
	currentSkill = E_SkillState::Skill_Null;
	if (myCharacter)
	{
		myCharacter->skillRange->SetVisibility(false);
	}
	else
	{
		myCharacter = Cast<ASidheRigelCharacter>(playerController->GetCharacter());
		UE_LOG(LogTemp,Error,TEXT("ERROR CHARACTER NULL IN STATEMACHINE"))
	}

	currentState->OnRightClick();
}

//Move
void UStateMachine::OnRightRelease()
{
	if (!bIsinitialized)
		return;
	currentState->OnRightRelease();
}

//SkillUse
void UStateMachine::OnLeftClick()
{
	if (!bIsinitialized)
		return;
	currentState->OnLeftClick();
}

//SkillReady
void UStateMachine::OnKeyboard(E_SkillState SkillState)
{
	if (!bIsinitialized)
		return;
	if (silenceTime > 0)
	{
		return;
	}

	currentSkill = SkillState;
	if (!myCharacter->skills.Contains(currentSkill))
	{
		currentSkill = E_SkillState::Skill_Null;
		return;
	}

	bSkillReady = true;
	float currentSkillRange = myCharacter->skills[currentSkill]->GetRange();
	myCharacter->skillRange->SetRelativeScale3D(FVector(currentSkillRange / 100, currentSkillRange / 100, 1));

	myCharacter->skillRange->SetVisibility(true);

	currentState->OnKeyboard(SkillState);
}

void UStateMachine::OnStun(float _stunTime)
{
	stunTime = _stunTime;
	ChangeState(Stun);
}

void UStateMachine::OnStop(float _stopTime)
{
	if(myCharacter)
		myCharacter->SpawnStopParticle();
	stopTime = _stopTime;
}

void UStateMachine::OnSilence(float _silenceTime)
{
	if (myCharacter)
		myCharacter->SpawnSilenceParticle();
	silenceTime = _silenceTime;
}

void UStateMachine::HasAttackEnemy()
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

void UStateMachine::ChangeCurrentSkill(E_SkillState SkillState)
{
	currentSkill = SkillState;
	if (!myCharacter->skills.Contains(currentSkill))
	{
		currentSkill = E_SkillState::Skill_Null;
		return;
	}

	auto& skill = myCharacter->skills[SkillState];
	if (skill == nullptr)
		return;

	//Check Cooldown, Mana
	if (skill->GetCooldown() <= 0 && skill->hasEnoughMana())
	{
		currentSkill = SkillState;
		bSkillReady = true;

		//Check Instant cast
		if (skill->IsInstantCast() && skill->CanUse())
		{
			ChangeState(UseSkill);
		}
	}
	else
	{
		currentSkill = E_SkillState::Skill_Null;
		bSkillReady = false;
		myCharacter->skillRange->SetVisibility(false);
	}
}

void UStateMachine::ChangeCharacterSpeed(float speed)
{
	myCharacter->GetCharacterMovement()->MaxWalkSpeed = speed;
}
