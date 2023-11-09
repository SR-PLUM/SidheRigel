// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../TalentQuest.h"
#include "KerunQSkillTalentQuest.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UKerunQSkillTalentQuest : public UTalentQuest
{
	GENERATED_BODY()

protected:
	int32 UpgradeQAttackCount = 4;

	UPROPERTY()
		class UKerunQSkill* QSkillRef;

public:
	inline void Initialize(class UKerunQSkill* QSkill);

	void InitTalentQuestProperty(class UKerunQSkill* QSkill);

	int32 GetQAttackCount();

	virtual void OnCompleted();
	
};
