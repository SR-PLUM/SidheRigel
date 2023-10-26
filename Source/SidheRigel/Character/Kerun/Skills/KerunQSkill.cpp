// Fill out your copyright notice in the Description page of Project Settings.

#include "KerunQSkill.h"
#include "../KerunCharacter.h"
#include "TimerManager.h"

void UKerunQSkill::QuitQSkill()
{
	character->RemoveAttackSpeed("QSkill");
	IsWorking = false;
	AttackCount = 0;
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Kerun QSkill End")));
}

bool UKerunQSkill::CheckAttackCount()
{
	return AttackCount >= MaxAttackCount;
}

void UKerunQSkill::SetSkillProperty(ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillDelay = 1.f;
	skillCooldown = 0;
	skillMaxCooldown = 10.f;

	bIsInstantCast = true;
	bIsTargeting = false;

	character = Character;
	skillstate = SkillState;
}

void UKerunQSkill::OnUse(FHitResult Hit)
{
	character->AddAttackSpeed("QSkill", MaxAttackSpeed);
	IsWorking = true;
}

void UKerunQSkill::OnTick(float DeltaTime)
{
	USkill::OnTick(DeltaTime);

	if (BuffDuration > 0)
	{
		BuffDuration -= DeltaTime;

		if (BuffDuration <= 0)
		{
			QuitQSkill();
		}
	}
}
