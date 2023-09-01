// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../SidheRigelPlayerController.h"


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

public:
	ASidheRigelPlayerController* playerController;
	AActor* target;
	FVector location;

	//Delay between attack to attack
	float attackDelay = 0;

	//Delay between cast to attack
	float frontDelay = 0;

	void ChangeState(State* NextState);

public:
	void Update(float DeltaTime);
	void OnRightClick();
	void OnRightRelease();
	void OnLeftClick();
	void OnKeyboard();
};
