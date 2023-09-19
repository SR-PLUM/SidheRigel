// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../SidheRigelPlayerController.h"
#include "../Enum/E_SkillState.h"


/**
 * 
 */
class SIDHERIGEL_API StateMachine
{
public:
	StateMachine(ASidheRigelPlayerController* PlayerController);
	~StateMachine();

private:
	class State* currentState;

public:
	class State* Idle;
	class State* MoveToAttack;
	class State* Move;
	class State* AttackWait;
	class State* Attack;
	class State* SkillReady;
	class State* UseSkill;

	class State* Stun;

public:
	ASidheRigelPlayerController* playerController;
	AActor* target;
	FVector location;

	//Delay between attack to attack
	float attackDelay = 0;

	//Delay between cast to attack
	float frontDelay = 0;

	//Delay During use Skill
	float SkillDelay = 0;

	//During Stun
	float stunTime = 0;

	//During Stop
	float stopTime = 0;

	bool bAttackWithSkillReady = false;

	//casting keyboard
	E_SkillState currentSkill = E_SkillState::Null;

	void ChangeState(State* NextState);

public:
	void Update(float DeltaTime);
	void OnRightClick();
	void OnRightRelease();
	void OnLeftClick();
	void OnKeyboard(E_SkillState SkillState);

	void OnStun(float stunTime);
};
