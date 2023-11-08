// Fill out your copyright notice in the Description page of Project Settings.


#include "TalentQuest.h"

void UTalentQuest::OnCompleted()
{
	IsCompleted = true;
}

void UTalentQuest::InitGoalPoint(int32 Point)
{
	GoalPoint = Point;
}

void UTalentQuest::IncreaseQuestGoal(int32 Value)
{
	CurrentPoint += Value;
}

bool UTalentQuest::GetQuestState()
{
	return IsCompleted;
}


