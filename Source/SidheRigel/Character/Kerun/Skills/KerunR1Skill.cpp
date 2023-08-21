// Fill out your copyright notice in the Description page of Project Settings.


#include "KerunR1Skill.h"

#include "../KerunCharacter.h"

#include "SidheRigel/Interface/Damagable.h"
#include "SidheRigel/Interface/CCable.h"

void UKerunR1Skill::StunTarget(AActor* Actor, AKerunCharacter* Owner)
{
	if (!(IsCoolingDown))
	{
		IsCoolingDown = true;

		if (IDamagable* Target = Cast<IDamagable>(Actor))
		{
			Target->TakeDamage(15.0f, Cast<AActor>(Owner));
		}

		if (ICCable* Target = Cast<ICCable>(Actor))
		{
			Target->Stun(2.0f);
		}

		Owner->GetWorldTimerManager().SetTimer(CoolingTimer,
			FTimerDelegate::CreateLambda([&]() {
				IsCoolingDown = false;
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Kerun R1Skill Cool Downed")));
				}), CoolDownDuration, false);
	}
	else if (IsCoolingDown)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Kerun R1Skill Cooling Down")));
	}

}
