// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/SidheRigelPlayerController.h"

USkill::USkill()
{
}

USkill::~USkill()
{
	UE_LOG(LogTemp,Warning, TEXT("DELETE USKILL"))
}

void USkill::SetSkillProperty(ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillDelay = 1.f;
	skillCooldown = 0;
	skillMaxCooldown = 2.f;
	range = 500.f;

	bIsInstantCast = false;
	bIsTargeting = true;

	character = Character;
	skillstate = SkillState;

	character->GetWorldTimerManager().SetTimer(cooldownTimer,this, &USkill::OnTick, 0.1f, true);
}

void USkill::OnTick()
{
	if (skillCooldown > 0)
	{
		skillCooldown -= 0.1f;

		if (skillCooldown <= 0)
		{
			character->ClearUISkillCoolDown(skillstate);
		}
		else
		{
			character->SetUISkillCoolDown(skillstate, skillCooldown / skillMaxCooldown, skillCooldown);
		}
	}
}

void USkill::OnUse(FHitResult Hit)
{
	//Implement Skill Detail
}

float USkill::GetSkillDelay()
{
	return skillDelay;
}

void USkill::SetCooldown()
{
	skillCooldown = skillMaxCooldown;
}

float USkill::GetCooldown()
{
	return skillCooldown;
}

bool USkill::IsInstantCast()
{
	return bIsInstantCast;
}

bool USkill::IsTargeting()
{
	return bIsTargeting;
}

bool USkill::CanUse()
{
	if (!bIsTargeting)
	{
		return true;
	}

	auto SRController = Cast<ASidheRigelPlayerController>(character->GetController());
	if (SRController)
	{
		auto hit = SRController->GetHitResult();

		if (character->GetDistanceTo(hit.GetActor()) > range)
			return false;

		auto teamActor = Cast<ITeam>(hit.GetActor());
		if (teamActor && teamActor->GetTeam() != character->GetTeam())
		{
			return true;
		}
	}

	return false;
}

float USkill::GetRange()
{
	return range;
}
