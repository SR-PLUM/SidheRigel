// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../Skill.h"
#include "ColdR2Skill.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UColdR2Skill : public USkill
{
	GENERATED_BODY()
public:
	UColdR2Skill();
	~UColdR2Skill();

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState) override;
	virtual void OnUse(FHitResult Hit) override;

private:
	float colliderDamage = 250.f;
	float colliderDuration = 1.f;

	TSubclassOf<class AColdR2Projectile> projectileClass;
	
};
