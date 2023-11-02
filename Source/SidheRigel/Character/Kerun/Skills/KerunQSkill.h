// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Skill.h"
#include "KerunQSkill.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UKerunQSkill : public USkill
{
	GENERATED_BODY()
public:

	virtual void SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState) override;
	virtual void OnUse(FHitResult Hit) override;
	virtual void OnTick() override;

private:	//Property

public :
	int32 AttackCount = 0;
	int32 MaxAttackCount = 2;

	float MaxDuration = 5.0f;
	float BuffDuration = 0.f;
	float MaxAttackSpeed = 2.5f;

	bool IsWorking = false;

private: // Talent
	
	float Kerun01DecreaseDefencePercent = 0.1f;
	float Kerun01DecreaseDefenceTime = 4.f;

	float Kerun02UpgradeAmount = 7.f;

public :
	
	void QuitQSkill();

	bool CheckAttackCount();

	void ApplyTalentWhenFullComboHits(AActor* target);

	float GetQDamage();
};
