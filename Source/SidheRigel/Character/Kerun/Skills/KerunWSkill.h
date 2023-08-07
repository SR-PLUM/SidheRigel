// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KerunWSkill.generated.h"
/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UKerunWSkill : public UObject
{
	GENERATED_BODY()

private:

	bool IsWorking = false;
	bool IsCoolingDown = false;
	FVector Velocity = FVector::ZeroVector;

	//CoolTime
	float CoolDownDuration = 10.f;

	//Cooling SetTimer
	FTimerHandle CoolingTimer;

public:
	void JumpIntoTarget(AActor* Actor, class AKerunCharacter* Owner);
	bool GetIsWorking();
	void SetIsWorking(bool flag);
	FVector GetVelocity();
};
