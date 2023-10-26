// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../Skill.h"
#include "ColdR1Skill.generated.h"
/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UColdR1Skill : public USkill
{
	GENERATED_BODY()
public:
	UColdR1Skill();
	~UColdR1Skill();

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState) override;
	virtual void OnUse(FHitResult Hit) override;

protected:
	float colliderDamage = 500.f;

	TSubclassOf<class AColdR1Projectile> projectileClass;
};
