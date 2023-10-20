// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../Skill.h"

/**
 * 
 */
class SIDHERIGEL_API FairyWingESkill : public Skill
{
public:
	FairyWingESkill();
	~FairyWingESkill();

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState) override;
	virtual void OnUse(FHitResult Hit) override;

private:	//Property
	float colliderSpeed = 500.f;
	float colliderDelay = 0.1f;
	float colliderDamage = 20.f;

	TSubclassOf<class AFairyWingEProjectile> projectileClass;
};
