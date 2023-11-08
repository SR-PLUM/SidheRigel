// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TalentQuest.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UTalentQuest : public UObject
{
	GENERATED_BODY()

protected:

	bool IsCompleted = false;
	int32 GoalPoint;
	int32 CurrentPoint = 0;
	
	void OnCompleted();

public:
	void IncreaseQuestGoal(int32 value);

	bool GetQuestState();

};
