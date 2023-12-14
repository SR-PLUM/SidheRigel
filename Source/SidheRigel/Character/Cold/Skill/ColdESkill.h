// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../Skill.h"
#include "ColdESkill.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UColdESkill : public USkill
{
	GENERATED_BODY()
public:
	UColdESkill();
	~UColdESkill();

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState) override;
	virtual void OnUse(FHitResult Hit) override;

	virtual float GetRange() override;
	virtual float GetSkillDelay() override;
	virtual float GetRequireMana() override;

private:
	TSubclassOf<class AColdEProjectile> projectileClass;
	TSubclassOf<class AColdEDamageField> damageFieldClass;

	float colliderFieldArea = 10.f;
	float colliderDamage = 50.f;
	float colliderFieldDamage = 10.f;

	float talentDamage = 70.f;
	float talentFieldDamage = 20.f;
	float talentMana = 70.f;
	float talentDelay = 0.5f;
	float talentRange = 1400.f;
};
