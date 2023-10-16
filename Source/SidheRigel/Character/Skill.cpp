// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/SidheRigelPlayerController.h"

Skill::Skill()
{
}

Skill::~Skill()
{
}

void Skill::SetSkillProperty(ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillDelay = 1.f;
	skillCooldown = 0;
	skillMaxCooldown = 2.f;
	range = 500.f;

	bIsInstantCast = false;
	bIsTargeting = true;

	character = Character;
	skillstate = SkillState;
}

void Skill::OnTick(float DeltaTime)
{
	if (skillCooldown > 0)
	{
		skillCooldown -= DeltaTime;

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

void Skill::OnUse(FHitResult Hit)
{
	//Implement Skill Detail
}

float Skill::GetSkillDelay()
{
	return skillDelay;
}

void Skill::SetCooldown()
{
	skillCooldown = skillMaxCooldown;
}

float Skill::GetCooldown()
{
	return skillCooldown;
}

bool Skill::IsInstantCast()
{
	return bIsInstantCast;
}

bool Skill::CanUse()
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

float Skill::GetRange()
{
	return range;
}
