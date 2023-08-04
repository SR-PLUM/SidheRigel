// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KerunR2Skill.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UKerunR2Skill : public UObject
{
	GENERATED_BODY()

private:
	bool IsWorking = false;
	bool IsCoolingDown = false;

	float Duration = 5.0f;
	float MinHP = 1.0f;

	//CoolTime
	float CoolDownDuration = 20.f;

private: //Timer
	//Working SetTimer
	FTimerHandle WorkingTimer;

	//Cooling SetTimer
	FTimerHandle CoolingTimer;

public:
	void StartR2Buff(class AKerunCharacter* Owner);

	bool GetIsWorking();

	bool CheckCurrentHP(float currentHP);

	float GetMinHP();

};
