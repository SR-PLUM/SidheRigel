// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../Skill.h"
#include "ColdWSkill.generated.h"
/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UColdWSkill : public USkill
{
	GENERATED_BODY()
public:
	UColdWSkill();
	~UColdWSkill();

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState) override;
	virtual void OnUse(FHitResult Hit) override;

	float GetSkillDelay() override;

private:
	float colliderDamage = 120.f;
	float colliderDuration = 2.f;
	float colliderForce = 2000.f;

	float talentDamage = 200.f;
	float talentDelay = 0.5f;

	TSubclassOf<class AColdWProjectile> projectileClass;
};
