// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../Skill.h"
/**
 * 
 */
class SIDHERIGEL_API BlackWizardWSkill : public Skill
{
public:
	BlackWizardWSkill();
	~BlackWizardWSkill();

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState) override;
	virtual void OnUse(FHitResult Hit) override;

	float force = 100000.0f;
};
