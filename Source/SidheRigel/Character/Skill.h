// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../Enum/E_SkillState.h"

/**
 * 
 */
class SIDHERIGEL_API Skill
{
public:
	Skill();
	~Skill();

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState);

	void OnTick(float DeltaTime);
	virtual void OnUse(FHitResult Hit);
	float GetSkillDelay();
	void SetCooldown();
	float GetCooldown();
	bool IsInstantCast();
	bool CanUse();
	float GetRange();

protected:
	float skillDelay;
	float skillCooldown;
	float skillMaxCooldown;
	float range;

	bool bIsInstantCast;
	bool bIsTargeting;

	class ASidheRigelCharacter* character;
	E_SkillState skillstate;
};
