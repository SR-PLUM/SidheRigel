// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../TalentQuest.h"
#include "KerunESkillTalentQuest.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UKerunESkillTalentQuest : public UTalentQuest
{
	GENERATED_BODY()

protected:
	int32 UpgradeEMaxCount = 35;

	int32 UpgradeEAddAmount = 2;

	UPROPERTY()
	class AKerunCharacter* Character;

public:
	void InitTalentQuestProperty(class AKerunCharacter* character);

	virtual void OnCompleted() override;
	
};
