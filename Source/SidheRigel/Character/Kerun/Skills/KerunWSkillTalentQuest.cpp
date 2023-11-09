// Fill out your copyright notice in the Description page of Project Settings.


#include "KerunWSkillTalentQuest.h"

void UKerunWSkillTalentQuest::InitTalentQuestProperty(AKerunCharacter* character)
{
	Character = character;

	GoalPoint = 20;
}

int32 UKerunWSkillTalentQuest::GetEStackAmount()
{
	return AddEStackAmount;
}
