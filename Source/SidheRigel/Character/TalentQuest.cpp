// Fill out your copyright notice in the Description page of Project Settings.


#include "TalentQuest.h"

void UTalentQuest::OnCompleted()
{
	IsCompleted = true;
}

void UTalentQuest::IncreaseQuestGoal(int32 Value)
{
	CurrentPoint += Value;

	if (CurrentPoint >= GoalPoint)
	{
		OnCompleted();
	}
}

bool UTalentQuest::GetQuestState()
{
	return IsCompleted;
}


