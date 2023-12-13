// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../Skill.h"
#include "FairyWingQSkill.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UFairyWingQSkill : public USkill
{
	GENERATED_BODY()
public:
	UFairyWingQSkill();
	~UFairyWingQSkill();

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState) override;
	virtual void OnUse(FHitResult Hit) override;

private:
	float colliderDamage = 50.f;
	float colliderDuration = 2.f;
	float colliderForce = 10000.f;
	float colliderRestoreHPValue = 50.f;
	float increaseColliderSpeed = 500.f;
	float increaseColliderSpeedTime = 5.f;

	float upgradeColliderDamage = 100.f;
	float upgradeColliderDuration = 4.f;
	float increaseColliderAttackSpeed = 1.f;	
	float colliderBlindTime = 1.f;

	TSubclassOf<class AFairyWingQCollider> colliderClass;
};
