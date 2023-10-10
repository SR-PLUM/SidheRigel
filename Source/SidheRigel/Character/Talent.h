#pragma once

#include "CoreMinimal.h"
#include "Talent.generated.h"

USTRUCT(BlueprintType)
struct FTalent
{
public:
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
		FString imgPath;

	UPROPERTY()
		FString talentDescription;

	UPROPERTY()
		FString talentName;
};