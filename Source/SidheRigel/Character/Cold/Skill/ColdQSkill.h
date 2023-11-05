// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../Skill.h"
#include "ColdQSkill.generated.h"
/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UColdQSkill : public USkill
{
	GENERATED_BODY()
public:
	UColdQSkill();
	~UColdQSkill();

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState) override;
	virtual void OnUse(FHitResult Hit) override;

	float GetRange() override;

private:	//Property
	float colliderSpeed = 500.f;
	float colliderDelay = 0.1f;
	float colliderDamage = 20.f;
	int32 count = 5;

	float talentRange = 400.f;
	int32 talentCount = 10;
	float talentSlow = 0.5f;
	float talentSlowTime = 2;
	

	TSubclassOf<class AColdQProjectile> projectileClass;
};
