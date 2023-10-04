// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../Skill.h"

/**
 * 
 */
class SIDHERIGEL_API ColdR2Skill : public Skill
{
public:
	ColdR2Skill();
	~ColdR2Skill();

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState) override;
	virtual void OnUse(FHitResult Hit) override;

protected:
	float colliderDamage = 500.f;

	TSubclassOf<class AColdR2Projectile> projectileClass;
};
