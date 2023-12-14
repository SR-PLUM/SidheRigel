// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../Skill.h"
#include "BlackWizardWSkill.generated.h"
/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UBlackWizardWSkill : public USkill
{
	GENERATED_BODY()
public:
	UBlackWizardWSkill();
	~UBlackWizardWSkill();

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState) override;
	virtual void OnUse(FHitResult Hit) override;

	float force = 100000.0f;
};
