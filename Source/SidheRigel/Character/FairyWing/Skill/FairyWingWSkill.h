// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../Skill.h"
#include "FairyWingWSkill.generated.h"
/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UFairyWingWSkill : public USkill
{
	GENERATED_BODY()
public:
	UFairyWingWSkill();
	~UFairyWingWSkill();

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState) override;
	virtual void OnUse(FHitResult Hit) override;

private:
	float colliderDamage = 50.f;
	float colliderDuration = 2.f;
	float colliderSlow = 0.5f;

	float upgradeColliderDamage = 100.f;
	float addColliderDamage = 50.f;
	float addColliderSlow = 0.7f;

	TSubclassOf<class AFairyWingWCollider> colliderClass;
};
