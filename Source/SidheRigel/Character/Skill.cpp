// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill.h"
#include "SidheRigel/SidheRigelCharacter.h"

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
	UE_LOG(LogTemp, Warning, TEXT("MAX COOL : %d"), skillMaxCooldown);
}

float Skill::GetCooldown()
{
	UE_LOG(LogTemp, Warning, TEXT("CUR COOL : %d"), skillCooldown);
	return skillCooldown;
}

bool Skill::IsInstantCast()
{
	return bIsInstantCast;
}
