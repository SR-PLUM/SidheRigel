// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "State.h"

/**
 * 
 */
class SIDHERIGEL_API UseSkillState : public State
{
public:
	UseSkillState(StateMachine* StateMachine);
	~UseSkillState();

public:
	void OnBegin() override;
	void Update(float DeltaTime) override;
	void OnRightClick() override;
	void OnRightRelease() override;
	void OnLeftClick() override;
	void OnKeyboard(E_SkillState SkillState) override;
	void OnEnd() override;
};
