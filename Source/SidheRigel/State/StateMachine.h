// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../Enum/E_SkillState.h"

#include "StateMachine.generated.h"


/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UStateMachine : public UObject
{
	GENERATED_BODY()

public:
	UStateMachine();
	~UStateMachine();
	void InitializeController(class ASidheRigelPlayerController* Controller);

private:
	class UState* currentState;
	class UState* previousState;

public:
	class UState* Idle;
	class UState* MoveToAttack;
	class UState* Move;
	class UState* AttackWait;
	class UState* Attack;
	class UState* UseSkill;

	//Special State
	class UState* Stun;
	class UState* Die;

	bool bIsinitialized = false;

public:
	class ASidheRigelPlayerController* playerController;
	class ASidheRigelCharacter* myCharacter;
	AActor* target;
	FVector location;

	bool bSkillReady = false;

	E_SkillState currentSkill = E_SkillState::Skill_Null;

public:	//Timer
	//Delay between attack to attack
	UPROPERTY()
		float attackDelay = 0;

	//Delay between cast to attack
	UPROPERTY()
		float frontDelay = 0;

	//Delay During use Skill
	UPROPERTY()
		float skillDelay = 0;

	//During Stun
	UPROPERTY()
		float stunTime = 0;

	//During Stop
	UPROPERTY()
		float stopTime = 0;

	//During Silence
	UPROPERTY()
		float silenceTime = 0;

	UPROPERTY()
		float DieTime = 0;

	void ChangeState(class UState* NextState);

	void ChangePreviousState();
	UState* GetCurrentState();

public:
	void Update();
	void OnRightClick();
	void OnRightRelease();
	void OnLeftClick();
	void OnKeyboard(E_SkillState SkillState);

	void OnStun(float _stunTime);
	void OnStop(float _stopTime);
	void OnSilence(float _silenceTime);

	void HasAttackEnemy();
	void ChangeCurrentSkill(E_SkillState SkillState);

	void ChangeCharacterSpeed(float _speed);
};
