// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../Skill.h"
/**
 * 
 */
class SIDHERIGEL_API ColdQSkill : public Skill
{
public:
	ColdQSkill();
	~ColdQSkill();

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character) override;
	virtual void OnUse(FHitResult Hit) override;

private:	//Property
	float colliderSpeed = 5.f;
	float colliderDelay = 0.1f;
	float colliderDamage = 20.f;
	int32 count = 10;

	TSubclassOf<class AColdQProjectile> projectileClass;
};
