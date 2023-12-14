// Fill out your copyright notice in the Description page of Project Settings.


#include "KerunR2Skill.h"
#include "../KerunCharacter.h"


void UKerunR2Skill::StartR2Buff(AKerunCharacter* Owner)
{
	if (!(IsWorking) && !(IsCoolingDown))
	{
		IsWorking = true;
		IsCoolingDown = true;

		Owner->GetWorldTimerManager().SetTimer(WorkingTimer,
			FTimerDelegate::CreateLambda([&]() {
				IsWorking = false;
				}), Duration, false);

		Owner->GetWorldTimerManager().SetTimer(CoolingTimer,
			FTimerDelegate::CreateLambda([&]() {
				IsCoolingDown = false;
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Kerun R2Skill Cool Downed")));
				}), CoolDownDuration, false);
	}
	else if (IsCoolingDown)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Kerun R2Skill Cooling Down")));
	}
}

bool UKerunR2Skill::GetIsWorking()
{
	return IsWorking;
}

bool UKerunR2Skill::CheckCurrentHP(float currentHP)
{
	if (currentHP < MinHP)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float UKerunR2Skill::GetMinHP()
{
	return MinHP;
}
