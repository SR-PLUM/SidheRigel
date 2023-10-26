// Fill out your copyright notice in the Description page of Project Settings.


#include "KerunR1Skill.h"

#include "../KerunCharacter.h"

#include "SidheRigel/Interface/Damagable.h"
#include "SidheRigel/Interface/CCable.h"

void UKerunR1Skill::SetSkillProperty(ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillDelay = 1.f;
	skillCooldown = 0.f;
	skillMaxCooldown = 20.f;
	range = 300.f;

	bIsInstantCast = false;
	bIsTargeting = true;

	character = Character;
	skillstate = SkillState;
}

void UKerunR1Skill::OnUse(FHitResult Hit)
{
	if (AActor* Actor = Hit.GetActor())
	{
		if (IDamagable* Target = Cast<IDamagable>(Actor))
		{
			Target->TakeDamage(15.0f, character);

			if (ICCable* CCTarget = Cast<ICCable>(Actor))
			{
				CCTarget->Stun(2.0f);
			}

			AKerunCharacter* KerunCharacter = Cast<AKerunCharacter>(character);

			KerunCharacter->ImproveEStack(6);
		}
	}
}
