#pragma once

#include "CoreMinimal.h"

#include "Talent.h"

#include "TalentList.generated.h"

USTRUCT(BlueprintType)
struct FTalentList
{
public:
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
		TArray<FTalent*> talentItems;

	UPROPERTY()
		int32 itemCount;
};