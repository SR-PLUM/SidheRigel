// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../Skill.h"
/**
 * 
 */
class SIDHERIGEL_API BlackWizardQSkill : public Skill
{
public:
	BlackWizardQSkill();
	~BlackWizardQSkill();

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character) override;
	virtual void OnUse(FHitResult Hit) override;

private:
	float colliderDamage = 120.f;
	float colliderDuration = 2.f;
	float colliderForce = 10000.f;

	TSubclassOf<class ABlackWizardQCollider> colliderClass;
};
