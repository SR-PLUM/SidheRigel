// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../Skill.h"

/**
 * 
 */
class SIDHERIGEL_API ColdESkill : public Skill
{
public:
	ColdESkill();
	~ColdESkill();

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character) override;
	virtual void OnUse(FHitResult Hit) override;

private:
	TSubclassOf<class AColdEProjectile> projectileClass;
	TSubclassOf<class AColdEDamageField> damageFieldClass;

	float colliderFieldArea = 105.f;
	float colliderDamage = 10.f;
};
