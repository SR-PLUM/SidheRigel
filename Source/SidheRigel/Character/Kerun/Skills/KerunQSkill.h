// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KerunQSkill.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UKerunQSkill : public UObject
{

	GENERATED_BODY()

public:
	UKerunQSkill();
	~UKerunQSkill();

public :
	int32 AttackCount = 0;
	int32 MaxAttackCount = 3;

	float MaxDuration = 5.0f;
	float MaxAttackSpeed = 2.5f;

	bool IsWorking = false;
	bool IsCoolingDown = false;

	// CoolTime
	float CoolDownDuration = 10.f;

public :
	void ImproveAttackSpeed(TMap<FString, float>& AttackSpeed);
};
