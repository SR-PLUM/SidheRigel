// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "State.h"

/**
 * 
 */
class SIDHERIGEL_API MoveState : public State
{
public:
	MoveState(StateMachine* StateMachine);
	~MoveState();

private:
	class ASidheRigelCharacter* myCharacter;

	//Duartion that Pressing Right Click
	float FollowTime = 0.f;
	//Decision interval
	float ShortPressThreshold = 0.1f;

	//Check ShortClick When Move
	bool bInputPressed = false;

public:
	void OnBegin() override;
	void Update(float DeltaTime) override;
	void OnRightClick() override;
	void OnRightRelease() override;
	void OnLeftClick() override;
	void OnKeyboard() override;
	void OnEnd() override;
};
