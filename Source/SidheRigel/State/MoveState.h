// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "State.h"

#include "MoveState.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UMoveState : public UState
{
	GENERATED_BODY()
public:
	UMoveState();
	~UMoveState();

private:
	class ASidheRigelCharacter* myCharacter;

	//Check ShortClick When Move
	bool bInputPressed = false;

public:
	virtual void OnBegin() override;
	virtual void Update(float DeltaTime) override;
	virtual void OnRightClick() override;
	virtual void OnRightRelease() override;
	virtual void OnLeftClick() override;
	virtual void OnKeyboard(E_SkillState SkillState) override;
	virtual void OnEnd() override;
};
