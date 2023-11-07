// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../Enum/E_SkillState.h"
#include "Skill.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API USkill : public UObject
{
	GENERATED_BODY()
public:
	USkill();
	~USkill();

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState);

	virtual void OnTick();
	virtual void OnUse(FHitResult Hit);
	virtual float GetSkillDelay();
	void SetCooldown();
	float GetCooldown();
	bool IsInstantCast();
	bool IsTargeting();
	bool CanUse();
	virtual float GetRange();
	bool hasEnoughMana();

protected:
	float skillDelay;
	float skillCooldown;
	float skillMaxCooldown;
	float range;
	float requireMana;

	bool bIsInstantCast;
	bool bIsTargeting;

	class ASidheRigelCharacter* character;
	E_SkillState skillstate;

	FTimerHandle cooldownTimer;
};
