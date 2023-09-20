// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../Skill.h"

/**
 * 
 */
class SIDHERIGEL_API ColdR1Skill : public Skill
{
public:
	ColdR1Skill();
	~ColdR1Skill();

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character) override;
	virtual void OnUse(FHitResult Hit) override;

protected:
	float colliderDamage = 500.f;

	TSubclassOf<class AColdR1Projectile> projectileClass;
};
