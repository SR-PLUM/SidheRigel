// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../TalentQuest.h"
#include "KerunWSkillTalentQuest.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UKerunWSkillTalentQuest : public UTalentQuest
{
	GENERATED_BODY()

protected:
	int32 AddEStackAmount = 5;

	UPROPERTY()
		class AKerunCharacter* Character;

public:
	void InitTalentQuestProperty(class AKerunCharacter* character);
	
	int32 GetEStackAmount();
};
