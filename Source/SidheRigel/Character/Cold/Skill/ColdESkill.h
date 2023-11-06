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

	virtual float GetRequireMana() override;

private:
	TSubclassOf<class AColdEProjectile> projectileClass;
	TSubclassOf<class AColdEDamageField> damageFieldClass;

	float colliderFieldArea = 105.f;
	float colliderDamage = 10.f;

	float talentDamage = 30.f;
	float talentMana = 30.f;
};
