// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "State.h"

#include "IdleState.generated.h"
/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UIdleState : public UState
{
	GENERATED_BODY()
public:
	UIdleState();
	~UIdleState();

public:
	void OnBegin() override;
	void Update(float DeltaTime) override;
	void OnRightClick() override;
	void OnRightRelease() override;
	void OnLeftClick() override;
	void OnKeyboard(E_SkillState SkillState) override;
	void OnEnd() override;

private:
	class ASidheRigelCharacter* myCharacter;
};
