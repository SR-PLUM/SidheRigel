// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill.h"

Skill::Skill()
{
}

Skill::~Skill()
{
}

void Skill::SetSkillProperty(ASidheRigelCharacter* Character)
{
	skillDelay = 1.f;
	skillCooldown = 0;
	skillMaxCooldown = 2.f;
	character = Character;
}

void Skill::OnTick(float DeltaTime)
{
	if (skillCooldown > 0)
	{
		skillCooldown -= DeltaTime;

		if (skillCooldown <= 0)
		{
			//TODO :: UI ~
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
