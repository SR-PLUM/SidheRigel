// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SIDHERIGEL_API State
{
public:
	State(StateMachine* StateMachine);
	~State();

protected:
	class StateMachine* stateMachine;

public:
	virtual void OnBegin() = 0;
	virtual void Update(float DeltaTime) = 0;
	virtual void OnRightClick() = 0;
	virtual void OnRightRelease() = 0;
	virtual void OnLeftClick() = 0;
	virtual void OnKeyboard() = 0;
	virtual void OnEnd() = 0;
};
