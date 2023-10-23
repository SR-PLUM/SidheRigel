// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Skill.h"

/**
 * 
 */
class SIDHERIGEL_API KerunQSkill : public Skill
{

public:

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState) override;
	virtual void OnUse(FHitResult Hit) override;
	virtual void OnTick(float DeltaTime) override;

private:	//Property

public :
	int32 AttackCount = 0;
	int32 MaxAttackCount = 2;

	float MaxDuration = 5.0f;
	float BuffDuration = 0.f;
	float MaxAttackSpeed = 2.5f;

	bool IsWorking = false;

public :
	
	void QuitQSkill();

	bool CheckAttackCount();
};
