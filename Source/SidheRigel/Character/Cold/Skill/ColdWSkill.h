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

	float colliderDamage = 141.f;
	float colliderDuration = 0.1f;
	float colliderForce = 4000.f;

	float talentDamage = 300.f;
	float talentDelay = 0.5f;
	
	float particleDuration = 2.f;

	TSubclassOf<class AColdWProjectile> projectileClass;
	TSubclassOf<class AColdWParticle> particleClass;
	TSubclassOf<class AColdWWall> wallClass;
};
