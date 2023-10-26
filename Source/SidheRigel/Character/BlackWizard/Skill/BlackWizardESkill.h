// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../Skill.h"
#include "BlackWizardESkill.generated.h"
/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UBlackWizardESkill : public USkill
{
	GENERATED_BODY()
public:
	UBlackWizardESkill();
	~UBlackWizardESkill();

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState) override;
	virtual void OnUse(FHitResult Hit) override;

private:
	float colliderDamage = 120.f;
	float colliderDuration = 2.f;
	float colliderForce = 10000.f;

	TSubclassOf<class ABlackWizardECollider> colliderClass;
};
