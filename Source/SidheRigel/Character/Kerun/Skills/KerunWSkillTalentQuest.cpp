// Fill out your copyright notice in the Description page of Project Settings.


#include "KerunWSkillTalentQuest.h"

inline void UKerunWSkillTalentQuest::Initialize()
{
	InitTalentQuestProperty();
}

void UKerunWSkillTalentQuest::InitTalentQuestProperty()
{
	GoalPoint = 20;
}

int32 UKerunWSkillTalentQuest::GetEStackAmount()
{
	return AddEStackAmount;
}
