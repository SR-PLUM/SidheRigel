// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KerunR1Skill.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UKerunR1Skill : public UObject
{
	GENERATED_BODY()

private:

	bool IsCoolingDown = false;

	//CoolTime
	float CoolDownDuration = 20.f;

	//Cooling SetTimer
	FTimerHandle CoolingTimer;

public:
	void StunTarget(AActor* Actor, class AKerunCharacter* Owner);


};
