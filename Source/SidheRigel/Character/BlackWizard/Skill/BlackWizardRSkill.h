// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../Skill.h"
#include "BlackWizardRskill.generated.h"
/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UBlackWizardRSkill : public USkill
{
	GENERATED_BODY()
public:
	UBlackWizardRSkill();
	~UBlackWizardRSkill();

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState) override;
	virtual void OnUse(FHitResult Hit) override;

private:
	float colliderDamage = 20.f;
	float colliderDuration = 1.f;
	float colliderForce = 10000.f;

	TSubclassOf<class ABlackWizardRCollider> colliderClass;

};
