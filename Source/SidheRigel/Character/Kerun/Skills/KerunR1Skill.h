// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Skill.h"
#include "KerunR1Skill.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UKerunR1Skill : public USkill
{
	GENERATED_BODY()

public:
	virtual void SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState) override;

	virtual void OnUse(FHitResult Hit) override;

};
