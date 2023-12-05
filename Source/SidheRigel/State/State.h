// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachine.h"

#include "State.generated.h"
/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UState : public UObject
{
	GENERATED_BODY()
public:
	UState();
	void InitStateMachine(UStateMachine* _stateMachine);
	~UState();

public:
	class UStateMachine* stateMachine;

public:
	virtual void OnBegin();
	virtual void Update(float DeltaTime);
	virtual void OnRightClick();
	virtual void OnRightRelease();
	virtual void OnLeftClick();
	virtual void OnKeyboard(E_SkillState SkillState);
	virtual void OnEnd();
};
