// Fill out your copyright notice in the Description page of Project Settings.

#include "KerunQSkill.h"
#include "../KerunCharacter.h"
#include "TimerManager.h"

void UKerunQSkill::ImproveAttackSpeed(TMap<FString, float>& AttackSpeed, AKerunCharacter * Owner)
{
	if (!(IsWorking) && !(IsCoolingDown))
	{
		AttackSpeed.Add("QSkill", MaxAttackSpeed);
		IsWorking = true;
		IsCoolingDown = true;
		
		Owner->GetWorldTimerManager().SetTimer(WorkingTimer,
			FTimerDelegate::CreateLambda([&]() {
				QuitQSkill(AttackSpeed);
			}), MaxDuration, false);

		Owner->GetWorldTimerManager().SetTimer(CoolingTimer,
			FTimerDelegate::CreateLambda([&]() {
				IsCoolingDown = false;
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Kerun QSkill Cool Downed")));
				}), CoolDownDuration, false);
	}
	else if (IsCoolingDown)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Kerun QSkill Cooling Down")));
	}
	
}

void UKerunQSkill::QuitQSkill(TMap<FString, float>& AttackSpeed)
{
	if (AttackSpeed.Find("QSkill"))
	{
		AttackSpeed.Remove("QSkill");
		IsWorking = false;
		AttackCount = 0;

		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Kerun QSkill End")));
	}
	
}

bool UKerunQSkill::CheckAttackCount()
{
	return AttackCount >= MaxAttackCount;
}


