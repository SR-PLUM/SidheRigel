// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../Enum/E_SkillState.h"


/**
 * 
 */
class SIDHERIGEL_API StateMachine
{
public:
	StateMachine(class ASidheRigelPlayerController* PlayerController);
	~StateMachine();

private:
	class State* currentState;
	class State* previousState;

public:
	class State* Idle;
	class State* MoveToAttack;
	class State* Move;
	class State* AttackWait;
	class State* Attack;
	class State* UseSkill;

	//Special State
	class State* Stun;

public:
	class ASidheRigelPlayerController* playerController;
	class ASidheRigelCharacter* myCharacter;
	AActor* target;
	FVector location;

	bool bSkillReady = false;

	E_SkillState currentSkill = E_SkillState::Skill_Null;

public:	//Timer
	//Delay between attack to attack
	float attackDelay = 0;

	//Delay between cast to attack
	float frontDelay = 0;

	//Delay During use Skill
	float skillDelay = 0;

	//During Stun
	float stunTime = 0;

	//During Stop
	float stopTime = 0;

	//During Silence
	float silenceTime = 0;

	void ChangeState(State* NextState);
	void ChangePreviousState();

public:
	void Update(float DeltaTime);
	void OnRightClick();
	void OnRightRelease();
	void OnLeftClick();
	void OnKeyboard(E_SkillState SkillState);

	void OnStun(float stunTime);
	void OnStop(float stopTime);
	void OnSilence(float silenceTime);

	void HasAttackEnemy();
	void ChangeCurrentSkill(E_SkillState SkillState);

	void ChangeCharacterSpeed(float speed);
};
