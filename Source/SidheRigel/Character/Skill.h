// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SIDHERIGEL_API Skill
{
public:
	Skill();
	~Skill();

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character);

	void OnTick(float DeltaTime);
	virtual void OnUse(FHitResult Hit);
	float GetSkillDelay();
	void SetCooldown();
	float GetCooldown();
	bool IsInstantCast();

protected:
	float skillDelay;
	float skillCooldown;
	float skillMaxCooldown;
	bool bIsInstantCast;

	class ASidheRigelCharacter* character;
};
